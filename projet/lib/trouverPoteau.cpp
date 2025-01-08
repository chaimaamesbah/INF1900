#include "trouverPoteau.h"

constexpr uint8_t NOMBRE_EMISSION_SON_GRAVE = 8;
constexpr uint8_t NOMBRE_EMISSION_SON_AIGU = 2;
constexpr uint8_t DUREE_JOUER_MUSIQUE_GRAVE = 150;
constexpr uint16_t DUREE_JOUER_MUSIQUE_AIGU = 750;
constexpr uint8_t DUREE_ARRET_MUSIQUE = 50;
constexpr uint8_t FREQUENCE_GRAVE = 100; 
constexpr uint8_t FREQUENCE_AIGU = 50;

volatile uint8_t TrouverPoteau::boutonRelacheDetecte_ = 0;

void TrouverPoteau::gererInterruption() {
    if (Bouton::estRelacher(PD2)) {
        boutonRelacheDetecte_ = 1;
    }
}

void TrouverPoteau::reinitialiserBoutonRelacheDetecte() {
    boutonRelacheDetecte_ = 0;
}

uint8_t TrouverPoteau::verifierPresencePoteau() {
    uint32_t sommeFrequence = 0;
    uint16_t compteurFrequence = 0;
    uint8_t moyenneFrequence = 0;


    while (capteurLigne_->obtenirNombreCapteursAllumes() != 0) {
        roue_->tournerSurPlaceGauche(42);
        _delay_ms(1);
        sommeFrequence += capteurDistance_->obtenirFrequence();
        compteurFrequence++;
    }

    while (!capteurLigne_->estSeulementMilieuActif()) {
        capteurLigne_->mettreAJourCapteurs();
        roue_->tournerSurPlaceDroite(40);
        _delay_ms(1);
        sommeFrequence += capteurDistance_->obtenirFrequence();
        compteurFrequence++;
    }

    roue_->arreter();
    _delay_ms(1);

    while (capteurLigne_->obtenirNombreCapteursAllumes() != 0) {
        roue_->tournerSurPlaceDroite(40);
        _delay_ms(1);
        sommeFrequence += capteurDistance_->obtenirFrequence();
        compteurFrequence++;
    }

    while (!capteurLigne_->estUneLigne()) {
        capteurLigne_->mettreAJourCapteurs();
        roue_->tournerSurPlaceGauche(40);
        _delay_ms(1);
        sommeFrequence += capteurDistance_->obtenirFrequence();
        compteurFrequence++;
    }

    roue_->arreter();
    _delay_ms(1);

    if (compteurFrequence != 0) {
        moyenneFrequence = sommeFrequence / compteurFrequence;
    } else {
        moyenneFrequence = 0;
    }

    return moyenneFrequence;
}

void TrouverPoteau::trouverLigne() {
    compteurToursApresExtremite++;

    while (!capteurLigne_->estTousCapteursEteints()) {
        capteurLigne_->mettreAJourCapteurs();
        roue_->tournerSurPlaceDroite(45);
        _delay_ms(1);
    }

    while (!capteurLigne_->estUneLigne()) {
        capteurLigne_->mettreAJourCapteurs();

        roue_->tournerSurPlaceDroite(45);
        _delay_ms(1);

    }
    roue_->arreter();
    _delay_ms(1);
}

void TrouverPoteau::jouerDeuxSonsAigus() {
    for (uint8_t i = 0; i < NOMBRE_EMISSION_SON_AIGU; i++) {
        musique_->jouerNote(Musique::NoteMIDI::NOTE_81);
        _delay_ms(DUREE_JOUER_MUSIQUE_AIGU);
        musique_->arreter();
        _delay_ms(DUREE_ARRET_MUSIQUE);
    }
}

void TrouverPoteau::jouerHuitSonsGraves() {
    for (uint8_t i = 0; i < NOMBRE_EMISSION_SON_GRAVE; i++) {
        musique_->jouerNote(Musique::NoteMIDI::NOTE_50);
        _delay_ms(DUREE_JOUER_MUSIQUE_GRAVE);
        musique_->arreter();
        _delay_ms(DUREE_ARRET_MUSIQUE);
        
    }
}

void TrouverPoteau::trouverPoteau() {
    capteurLigne_->mettreAJourCapteurs();
    switch (etatSuivant) {
        case TrouverPoteauEtats::ETAT_INITIAL : {
            DEBUG_PRINT("État actuel : ETAT_INITIAL\n");

            _delay_ms(2000);
            reinitialiserAttributs();

            etatSuivant = TrouverPoteauEtats::SUIVRE_LIGNE;
        }

        case TrouverPoteauEtats::SUIVRE_LIGNE : {
            DEBUG_PRINT("État actuel : SUIVRE_LIGNE\n");
            
            if (compteurPoteauDetecte_ <= 2 && capteurDistance_->estPoteauDetecteDixCm()) {
                    etatSuivant = TrouverPoteauEtats::POTEAU_DETECTER_DIX_CM;
                }

            if (capteurLigne_->estTousCapteursEteints() && compteurCapteurDroitEteint_ == 1 && (compteurIntersection_ == 1 || compteurIntersection_ == 2 ) && !actionTournerDroiteEffectuee_) {
                    while (!capteurLigne_->estUneLigne()) {
                        capteurLigne_->mettreAJourCapteurs();
                        roue_->tournerSurPlaceDroite(43);
                        _delay_ms(1);
                    }
                    actionTournerDroiteEffectuee_ = true;
                }

            if (capteurLigne_->obtenirNombreCapteursAllumes() >= 3) { 
                etatSuivant = TrouverPoteauEtats::VERIFIER_CAPTEURS_ALLUME;
            }

            capteurLigne_->avancerEnSuivantLigne();
            _delay_ms(1);
            break;
        }

        case TrouverPoteauEtats::VERIFIER_CAPTEURS_ALLUME : {
            DEBUG_PRINT("État actuel : VERIFIER_CAPTEURS_ALLUME\n");
            bool estDroiteActif = true;
            bool estGaucheActif = true;
            uint8_t maxCapteursAllumes = 0; 
            uint8_t minCapteursAllumes = 5;

            capteurLigne_->verifierCapteursAllume(maxCapteursAllumes, minCapteursAllumes, estGaucheActif, estDroiteActif);

            if (maxCapteursAllumes == 5 && minCapteursAllumes == 0) {
                etatSuivant = TrouverPoteauEtats::VERIFIER_EXTREMITE_GAUCHE;
            } else if (maxCapteursAllumes == 5 && (minCapteursAllumes <= 2)) {
                compteurIntersection_++;
                if (compteurPoteauDetecte_ <= 2) {
                    etatSuivant = TrouverPoteauEtats::VERIFIER_PRESENCE_POTEAU;
                } else if (compteurPoteauDetecte_ == 3){
                    etatSuivant = TrouverPoteauEtats::SUIVRE_LIGNE;
                }

            } else if ((maxCapteursAllumes == 3 || maxCapteursAllumes == 4 || maxCapteursAllumes == 2) && minCapteursAllumes <= 2) {
                if (!estGaucheActif) {
                    etatSuivant = TrouverPoteauEtats::CAPTEUR_GAUCHE_ETEINT;
                } else if (!estDroiteActif) {
                    etatSuivant = TrouverPoteauEtats::CAPTEUR_DROIT_ETEINT;
                }
            } else {
                etatSuivant = TrouverPoteauEtats::SUIVRE_LIGNE;
            }
            break;
        }

        case TrouverPoteauEtats::VERIFIER_PRESENCE_POTEAU : {
            DEBUG_PRINT("État actuel : VERIFIER_PRESENCE_POTEAU\n");
            uint8_t moyenneFrequence = 0;
            roue_->arreter();
            _delay_ms(10);
            moyenneFrequence = verifierPresencePoteau();

            if (moyenneFrequence >= 10) {
                etatSuivant = TrouverPoteauEtats::SUIVRE_LIGNE;
            } else {
                etatSuivant = TrouverPoteauEtats::TROUVER_LIGNE;
            }
            break;
        }

        case TrouverPoteauEtats::TROUVER_LIGNE : {
            DEBUG_PRINT("État actuel : TROUVER_LIGNE\n");
            trouverLigne();

            etatSuivant = TrouverPoteauEtats::VERIFIER_PRESENCE_POTEAU;
            break;
        }

        case TrouverPoteauEtats::POTEAU_DETECTER_DIX_CM: {
                DEBUG_PRINT("État actuel : POTEAU_DETECTER_DIX_CM\n");
                roue_->arreter();
                _delay_ms(1);

                jouerHuitSonsGraves();
                mettreAJourPositionEtPositionPoteau();
                reinitialiserBoutonRelacheDetecte();

                compteurPoteauDetecte_++;
                break;
            }

        case TrouverPoteauEtats::VERIFIER_EXTREMITE_GAUCHE : {
            DEBUG_PRINT("État actuel : VERIFIER_EXTREMITE_GAUCHE\n");
            bool estLigneTrouverGauche = false;
            capteurLigne_->verifierExtremiteGauche(estLigneTrouverGauche);

            if (estLigneTrouverGauche) {
                compteurTournerGauche_++;
                etatSuivant = TrouverPoteauEtats::SUIVRE_LIGNE;
            } else if (capteurLigne_->estTousCapteursEteints() && !estLigneTrouverGauche) {
                etatSuivant = TrouverPoteauEtats::VERIFIER_EXTREMITE_DROITE;
            }
            break;
        }

        case TrouverPoteauEtats::VERIFIER_EXTREMITE_DROITE : {
            DEBUG_PRINT("État actuel : VERIFIER_EXTREMITE_DROITE\n");

            bool estLigneTrouverDroite = false;
            capteurLigne_->verifierExtremiteDroite(estLigneTrouverDroite);

            if (estLigneTrouverDroite) {
                compteurTournerDroite_++;
                etatSuivant = TrouverPoteauEtats::SUIVRE_LIGNE;
            } 
            else if (capteurLigne_->estTousCapteursEteints() && !estLigneTrouverDroite) {
                etatSuivant = TrouverPoteauEtats::EXTREMITE_TROUVER;
            }
            break;
        }

        case TrouverPoteauEtats::CAPTEUR_GAUCHE_ETEINT :{
            DEBUG_PRINT("État actuel : CAPTEUR_GAUCHE_ETEINT\n");
            capteurLigne_->ajusterTrajectoire(true);
            compteurCapteurGaucheEteint_++;
            etatSuivant = TrouverPoteauEtats::SUIVRE_LIGNE;
            break;
        }

        case TrouverPoteauEtats::CAPTEUR_DROIT_ETEINT :{
            DEBUG_PRINT("État actuel : CAPTEUR_DROIT_ETEINT\n");
            capteurLigne_->ajusterTrajectoire(false);
            compteurCapteurDroitEteint_++;
            etatSuivant = TrouverPoteauEtats::SUIVRE_LIGNE;
            break;
        }

        case TrouverPoteauEtats::EXTREMITE_TROUVER : {
            DEBUG_PRINT("État actuel : EXTREMITE_TROUVER\n");
            mettreAJourPositionEtPositionPoteau();
            capteurLigne_->replacerCorrectementExtremite();
            extremiteTrouvee_ = true;
            break;
        }
    }
}

void TrouverPoteau::reinitialiserAttributs() {
    DEBUG_PRINT("Réinitialisation des attributs de Trouver Poteau !\n");
    compteurTournerGauche_ = 0;
    compteurTournerDroite_ = 0;
    compteurCapteurGaucheEteint_ = 0;
    compteurCapteurDroitEteint_ = 0;
    compteurIntersection_ = 0;
    compteurToursApresExtremite = 0;
    actionTournerDroiteEffectuee_ = false;
}

void TrouverPoteau::mettreAJourPositionEtPositionPoteau() {
    DEBUG_PRINT("Mise à jour des positions\n");
    if (compteurPoteauDetecte_ == 1 && compteurCapteurGaucheEteint_ == 1 && compteurToursApresExtremite == 0) {
        pointDepart_ = POINT_D;
        positionPoteau1_ = 1;
    } else if (compteurPoteauDetecte_ == 1 && compteurCapteurDroitEteint_ == 1 && compteurToursApresExtremite >= 1) {
        pointDepart_ = POINT_C;
        positionPoteau1_ = 1;
    } else if (compteurPoteauDetecte_ == 1 && compteurTournerGauche_ == 1 && compteurToursApresExtremite >= 1) {
        pointDepart_ = POINT_D;
        positionPoteau1_ = 2;
    } else if (compteurPoteauDetecte_ == 1 && compteurTournerGauche_ == 1 && compteurToursApresExtremite == 0) {
        pointDepart_ = POINT_C;
        positionPoteau1_ = 2;
    } else if (compteurPoteauDetecte_ == 2 && compteurTournerDroite_ == 1) {
        positionPoteau2_ = 3;
    } else if (compteurPoteauDetecte_ == 2 && compteurTournerGauche_ == 1) {
        positionPoteau2_ = 4;
    } else if (compteurPoteauDetecte_ == 3 && compteurTournerDroite_ == 1) {
        positionPoteau3_ = 5;
    } else if (compteurPoteauDetecte_ == 3 && compteurCapteurDroitEteint_ == 1) {
        positionPoteau3_ = 6;
    }
}

void TrouverPoteau::demarrer() {
    DEBUG_PRINT("Début du programme : Trouver Poteau!\n");
    del_->eteindre();
    reinitialiserAttributs();
    extremiteTrouvee_ = false;
    compteurPoteauDetecte_ = 0;


    while (!extremiteTrouvee_) {
        if (boutonRelacheDetecte_ == 1) {
            cli();
            trouverPoteau();
            sei();
        } else {
            etatSuivant = TrouverPoteauEtats::ETAT_INITIAL;
            del_->eteindre();
        }
    }

    jouerDeuxSonsAigus();
    rapport_->definirpointDepartParcours(pointDepart_);
    rapport_->definirPoteau1(positionPoteau1_);
    rapport_->definirPoteau2(positionPoteau2_);
    rapport_->definirPoteau3(positionPoteau3_);
}
