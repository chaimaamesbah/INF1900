#include "chercherExtremite.h"

void ChercherExtremite::trouverExtremite() {
    capteurLigne_->mettreAJourCapteurs();
    if (!extremiteTrouvee_) {
        switch (etat) {
            case ChercherExtremiteEtats::SUIVRE_LIGNE : {
                DEBUG_PRINT("État actuel : SUIVRE_LIGNE\n");

                if (capteurLigne_->estTousCapteursEteints() && compteurCapteurDroitEteint_ == 1 && (compteurIntersection_ == 1 || compteurIntersection_ == 2 ) && !actionTournerDroiteEffectuee_) {
                        while (!capteurLigne_->estUneLigne()) {
                            capteurLigne_->mettreAJourCapteurs();
                            roue_->tournerSurPlaceDroite(43);
                            _delay_ms(1);
                        }
                        actionTournerDroiteEffectuee_ = true;
                    }

                if (capteurLigne_->obtenirNombreCapteursAllumes() >= 3) { 
                    etat = ChercherExtremiteEtats::VERIFIER_CAPTEURS_ALLUME;
                }

                capteurLigne_->avancerEnSuivantLigne();
                _delay_ms(1);
                break;
            }

            case ChercherExtremiteEtats::VERIFIER_CAPTEURS_ALLUME : {
                DEBUG_PRINT("État actuel : VERIFIER_CAPTEURS_ALLUME\n");
                bool estDroiteActif = true;
                bool estGaucheActif = true;
                uint8_t maxCapteursAllumes = 0; 
                uint8_t minCapteursAllumes = 5;

                
                capteurLigne_->verifierCapteursAllume(maxCapteursAllumes, minCapteursAllumes, estGaucheActif, estDroiteActif);
            
                if (maxCapteursAllumes == 5 && minCapteursAllumes == 0) {
                    etat = ChercherExtremiteEtats::VERIFIER_EXTREMITE_GAUCHE;
                } else if (maxCapteursAllumes == 5 && (minCapteursAllumes <= 2)) {
                    compteurIntersection_++;
                    etat = ChercherExtremiteEtats::SUIVRE_LIGNE;
                } else if ((maxCapteursAllumes == 3 || maxCapteursAllumes == 4 || maxCapteursAllumes == 2) && minCapteursAllumes <= 2) {
                    if (!estGaucheActif) {
                        etat = ChercherExtremiteEtats::CAPTEUR_GAUCHE_ETEINT;
                    } else if (!estDroiteActif) {
                        etat = ChercherExtremiteEtats::CAPTEUR_DROIT_ETEINT;
                    }
                } else {
                    etat = ChercherExtremiteEtats::SUIVRE_LIGNE;
                }

                break;
            }

            case ChercherExtremiteEtats::VERIFIER_EXTREMITE_GAUCHE : {
                DEBUG_PRINT("État actuel : VERIFIER_EXTREMITE_GAUCHE\n");
                bool estLigneTrouverGauche = false;
                capteurLigne_->verifierExtremiteGauche(estLigneTrouverGauche);

                if (estLigneTrouverGauche) {
                    compteurTournerGauche_++;
                    etat = ChercherExtremiteEtats::SUIVRE_LIGNE;
                } else if (capteurLigne_->estTousCapteursEteints() && !estLigneTrouverGauche) {
                    etat = ChercherExtremiteEtats::VERIFIER_EXTREMITE_DROITE;
                }
                break;
            }

            case ChercherExtremiteEtats::VERIFIER_EXTREMITE_DROITE : {
                DEBUG_PRINT("État actuel : VERIFIER_EXTREMITE_DROITE\n");

                bool estLigneTrouverDroite = false;
                capteurLigne_->verifierExtremiteDroite(estLigneTrouverDroite);

                if (estLigneTrouverDroite) {
                    compteurTournerDroite_++;
                    etat = ChercherExtremiteEtats::SUIVRE_LIGNE;
                } 
                else if (capteurLigne_->estTousCapteursEteints() && !estLigneTrouverDroite) {
                    etat = ChercherExtremiteEtats::EXTREMITE_TROUVER;
                }
                break;
            }

            case ChercherExtremiteEtats::CAPTEUR_GAUCHE_ETEINT :{
                DEBUG_PRINT("État actuel : CAPTEUR_GAUCHE_ETEINT\n");
                capteurLigne_->ajusterTrajectoire(true);
                compteurCapteurGaucheEteint_++;
                etat = ChercherExtremiteEtats::SUIVRE_LIGNE;
                break;
            }

            case ChercherExtremiteEtats::CAPTEUR_DROIT_ETEINT :{
                DEBUG_PRINT("État actuel : CAPTEUR_DROIT_ETEINT\n");
                capteurLigne_->ajusterTrajectoire(false);
                compteurCapteurDroitEteint_++;
                etat = ChercherExtremiteEtats::SUIVRE_LIGNE;
                break;
            }

            case ChercherExtremiteEtats::EXTREMITE_TROUVER : {
                DEBUG_PRINT("État actuel : EXTREMITE_TROUVER\n");
                capteurLigne_->replacerCorrectementExtremite();
                extremiteTrouvee_ = true;
                break;
            }
        }
    }
}

void ChercherExtremite::reinitialiserCompteurs() {
    DEBUG_PRINT("Réinitialisation des compteurs\n");
    compteurTournerGauche_ = 0;
    compteurTournerDroite_ = 0;
    compteurCapteurGaucheEteint_ = 0;
    compteurCapteurDroitEteint_ = 0;
    compteurIntersection_ = 0;
    actionTournerDroiteEffectuee_ = false;
}

void ChercherExtremite::definirPositionEtOrientation(Point point, Orientation orientation, Point extremite) {
    pointDepart_ = point;
    orientation_ = orientation;
    extremiteTrouver_ = extremite;

    char buffer[50];
    sprintf(buffer, "MAJ - Pt: %s, Ori: %s, Ext: %s\n", 
            rapport_->pointEnString(point), 
            rapport_->orientationEnString(orientation), 
            rapport_->pointEnString(extremite));
    DEBUG_PRINT(buffer);
}

void ChercherExtremite::mettreAJourPositionEtOrientation() {
    if (compteurTournerDroite_ == 1 && compteurCapteurDroitEteint_ == 1) {
        definirPositionEtOrientation(POINT_A, NORD_EST, POINT_E); // point 3 et point 6 (A - NE) 
    }
    else if (compteurCapteurDroitEteint_ == 1 && compteurCapteurGaucheEteint_ == 1) {
        definirPositionEtOrientation(POINT_B, NORD_OUEST, POINT_C); // point 3 et point 2 (B - NO)
    }
    else if (compteurTournerGauche_ == 1 && compteurTournerDroite_ == 1) {
        definirPositionEtOrientation(POINT_A, SUD_EST, POINT_E); // point 4 et point 5 (A - SE)
    }
    else if (compteurCapteurGaucheEteint_ == 1 && compteurTournerGauche_ == 1) {
        definirPositionEtOrientation(POINT_B, SUD_OUEST, POINT_D); // point 4 et point 1 (B - S0)
    }
    else if (compteurTournerGauche_ == 1) {
        definirPositionEtOrientation(POINT_A, NORD_OUEST, POINT_D); // point 1 (A - N0)
    }
    else if (compteurCapteurGaucheEteint_ == 1) {
        definirPositionEtOrientation(POINT_A, SUD_OUEST, POINT_C); // point 2 (A - S0)
    }
    else if (compteurTournerDroite_ == 1) {
        definirPositionEtOrientation(POINT_B, NORD_EST, POINT_E); // point 5 (B - NE)
    }
    else if (compteurCapteurDroitEteint_ == 1) {
        definirPositionEtOrientation(POINT_B, SUD_EST, POINT_E); // point 6 (B - SE)
    }
}

void ChercherExtremite::extremiteAtteinte() {
    mettreAJourPositionEtOrientation();
    DEBUG_PRINT("\ncompteurTournerGauche_ : ");
    DEBUG_PRINT(compteurTournerGauche_);
    DEBUG_PRINT("\ncompteurTournerDroite_ : ");
    DEBUG_PRINT(compteurTournerDroite_);
    DEBUG_PRINT("\ncompteurCapteurDroitEteint_ : ");
    DEBUG_PRINT(compteurCapteurDroitEteint_);
    DEBUG_PRINT("\ncompteurCapteurGaucheEteint_ : ");
    DEBUG_PRINT(compteurCapteurGaucheEteint_);
    DEBUG_PRINT("\n");

    DEBUG_PRINT("Extrémité atteinte, émission de sons et éclairage de la DEL_\n");
    musique_->jouerNote(117); // à changer
    _delay_ms(750);
    musique_->arreter();
    _delay_ms(50);
    musique_->jouerNote(117);
    _delay_ms(750);
    musique_->arreter();
    _delay_ms(50);

    switch (extremiteTrouver_) {
    case POINT_C : del_->allumer(VERT, 2000); break;
    case POINT_D : del_->allumer(ROUGE, 2000); break;
    case POINT_E : del_->allumer(AMBRE, 2000); break;
    default : break;
    }
}

void ChercherExtremite::demarrer() {
    DEBUG_PRINT("Début du programme : Chercher une extrémité\n");
    extremiteTrouvee_ = false;
    del_->eteindre();
    reinitialiserCompteurs();

    while (!extremiteTrouvee_) {
        trouverExtremite();
    }
    
    extremiteAtteinte();
    rapport_->definirpointDepartExtremite(pointDepart_);
    rapport_->definirOrientation(orientation_);
    rapport_->definirExtremiteTrouvee(extremiteTrouver_);
}