#include "modeFonctionnement.h"

Mode ModeFonctionnement::verifierReset() {
    uint8_t modeEnMemoire;
    if (MCUSR & (1 << EXTRF)) {
        memoire_->lecture(0x0000, &modeEnMemoire);
        MCUSR = 0;
        return static_cast<Mode>(modeEnMemoire);
    }
    MCUSR = 0;
    return Mode::AUCUN_MODE;
}

void ModeFonctionnement::enregistrerMode(Mode mode) {
    memoire_->ecriture(0x0000, static_cast<uint8_t>(mode));
}

void ModeFonctionnement::lancerCycleSelection() {
    while (true) {
        enregistrerMode(Mode::CHERCHER_EXTREMITE);
        del_->allumer(VERT, 2000);

        enregistrerMode(Mode::TRAVERSER_PARCOURS);
        del_->allumer(ROUGE, 2000);

        enregistrerMode(Mode::AFFICHER_RAPPORT);
        del_->allumer(AMBRE, 2000);
    }
}

