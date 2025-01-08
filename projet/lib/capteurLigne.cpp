#include "capteurLigne.h"

bool CapteurLigne::estGaucheActif = false;
bool CapteurLigne::estGaucheMilieuActif = false;
bool CapteurLigne::estMilieuActif = false;
bool CapteurLigne::estDroiteMilieuActif = false;
bool CapteurLigne::estDroiteActif = false;

CapteurLigne::CapteurLigne(Roue*roue) : roue_(roue) {
    DDRC &= ~(MASQUE_GAUCHE | MASQUE_GAUCHE_MILIEU | MASQUE_MILIEU);
    DDRC &= ~(MASQUE_DROITE_MILIEU | MASQUE_DROITE);
}

void CapteurLigne::mettreAJourCapteurs() {
    uint8_t etatCapteurs = PINC;
    estGaucheActif = (etatCapteurs & MASQUE_GAUCHE) != 0;
    estGaucheMilieuActif = (etatCapteurs & MASQUE_GAUCHE_MILIEU) != 0;
    estMilieuActif = (etatCapteurs & MASQUE_MILIEU) != 0;
    estDroiteMilieuActif = (etatCapteurs & MASQUE_DROITE_MILIEU) != 0;
    estDroiteActif = (etatCapteurs & MASQUE_DROITE) != 0;
}

uint8_t CapteurLigne::obtenirNombreCapteursAllumes() {
    mettreAJourCapteurs();
    uint8_t nombreActifs = 0;
    if (estGaucheActif) nombreActifs++;
    if (estGaucheMilieuActif) nombreActifs++;
    if (estMilieuActif) nombreActifs++;
    if (estDroiteMilieuActif) nombreActifs++;
    if (estDroiteActif) nombreActifs++;

    return nombreActifs;
}

bool CapteurLigne::estUneLigne() {
    return (estMilieuActif || estDroiteMilieuActif || estGaucheMilieuActif)
            && !estDroiteActif && !estGaucheActif;
}

bool CapteurLigne::estSeulementMilieuActif() {
    return estMilieuActif && !estDroiteMilieuActif && !estGaucheMilieuActif
            && !estDroiteActif && !estGaucheActif;
}

bool CapteurLigne::estCinqCapteursAllumes() {
    return estMilieuActif && estDroiteMilieuActif && estDroiteActif &&
        estGaucheActif && estGaucheMilieuActif;
}

bool CapteurLigne::estTroisCapteursDroiteActif() {
    return estMilieuActif && estDroiteMilieuActif && estDroiteActif && !estGaucheActif;
}

bool CapteurLigne::estTroisCapteursGaucheActif() {
    return estMilieuActif && estGaucheMilieuActif && estGaucheActif && !estDroiteActif;
}

bool CapteurLigne::estTousCapteursEteints() {
    return !estMilieuActif && !estDroiteMilieuActif && !estDroiteActif &&
            !estGaucheActif && !estGaucheMilieuActif;
}

void CapteurLigne::avancerEnSuivantLigne(uint8_t vitesse) {
    mettreAJourCapteurs();
    uint8_t vitesseAugmenterGauche = vitesse + 5;
    uint8_t vitesseAugmenterDroite = vitesse + 10;
    uint8_t vitesseReduite = vitesse - 5;

    if (estTousCapteursEteints()) {
        roue_->arreter();
        _delay_ms(1);
    }
    else if (estMilieuActif) {
        roue_->avancer(vitesse);

    }
    else if (estDroiteMilieuActif || estDroiteActif) {
        roue_->tourner(vitesseAugmenterGauche, vitesseReduite);
    } else if (estGaucheMilieuActif || estGaucheActif) {
        roue_->tourner(vitesseReduite, vitesseAugmenterDroite);
    } else {
        roue_->avancer(vitesse);
    }
}

void CapteurLigne::verifierCapteursAllume(uint8_t& maxCapteursAllumes_, uint8_t& minCapteursAllumes_, 
                                            bool& estCapteurGaucheActif, bool& estCapteurDroiteActif) {
    for (int i = 0; i < 2000; i++) { 
        uint8_t nbActifs = obtenirNombreCapteursAllumes();

        if (nbActifs > maxCapteursAllumes_) {
            maxCapteursAllumes_ = nbActifs;
            
            if (!estDroiteActif) {
                estCapteurDroiteActif = false;
            }
            else if (!estGaucheActif) {
                estCapteurGaucheActif = false;
            }
        }
        else if (nbActifs < minCapteursAllumes_) {
            minCapteursAllumes_ = nbActifs;
            if (minCapteursAllumes_ == 0) { break; }
        }

        avancerEnSuivantLigne(35);
        _delay_ms(1);
    }

    DEBUG_PRINT("Maximum capteurs allumés : ");
    DEBUG_PRINT(maxCapteursAllumes_);
    DEBUG_PRINT("\nMinimum capteurs allumés : ");
    DEBUG_PRINT(minCapteursAllumes_);
    DEBUG_PRINT("\n");
}

void CapteurLigne::verifierExtremiteGauche(bool& estLigneTrouverGauche) {
    roue_->avancer(45);
    _delay_ms(1500);

    for (int i = 0; i < 1500; i++) {
        roue_->tournerSurPlaceGauche(45);
        _delay_ms(1);

        mettreAJourCapteurs();
        if (estUneLigne()) {
            estLigneTrouverGauche = true;

            for (int t = 0; t < 500; t++) {
                avancerEnSuivantLigne();
                _delay_ms(1);

                mettreAJourCapteurs();
                if (estTousCapteursEteints()) {
                    DEBUG_PRINT("Capteurs éteints, ce n'est pas une ligne continue.\n");
                    estLigneTrouverGauche = false;
                    break;
                }
            }
            if (estLigneTrouverGauche) {
                DEBUG_PRINT("Ligne continue confirmée à gauche.\n");
                break;
            }
        }
    }
}

void CapteurLigne::remettreMilieu(bool estDirectionDroite) {
    for (int i = 0; i < 1400; i++) {
        if (estDirectionDroite) roue_->tournerSurPlaceDroite(45);
        else roue_->tournerSurPlaceGauche(45);
        _delay_ms(1);
    }
}

void CapteurLigne::verifierExtremiteDroite(bool& estLigneTrouverDroite) {
    for (int i = 0; i < 3500; i++) {
        roue_->tournerSurPlaceDroite(45);
        _delay_ms(1);

        mettreAJourCapteurs();
        if (estUneLigne()) {
            estLigneTrouverDroite = true;

            for (int t = 0; t < 500; t++) {
                avancerEnSuivantLigne();
                _delay_ms(1);

                mettreAJourCapteurs();
                if (estTousCapteursEteints()) {
                    estLigneTrouverDroite = false;
                    break;
                }
            }

            if (estLigneTrouverDroite) {
                break;
            }
        }
    }
}

void CapteurLigne::ajusterTrajectoire(bool estDirectionDroite) {
    while (!estTousCapteursEteints()) {
        avancerEnSuivantLigne(); 
        _delay_ms(1);
        mettreAJourCapteurs();
    }

    while (!estUneLigne()) {
        if (estDirectionDroite) roue_->tournerSurPlaceDroite(42);
        else roue_->tournerSurPlaceGauche(42);

        _delay_ms(1);
        mettreAJourCapteurs();
    }
}

void CapteurLigne::tournerDroiteApresPointSix() {
    while (true) {
        DEBUG_PRINT("Tourner à droite (provenance: point 6 !)");
        roue_->tournerSurPlaceDroite(38);
        _delay_ms(1);

        mettreAJourCapteurs();
        if (estUneLigne()) { break; }
    }
}

void CapteurLigne::replacerCorrectementExtremite() {
    remettreMilieu(false);

    while (obtenirNombreCapteursAllumes() == 0) {
        mettreAJourCapteurs();
        roue_->reculer(40);
        _delay_ms(1);
    }

    roue_->arreter();
    _delay_ms(1);
}
