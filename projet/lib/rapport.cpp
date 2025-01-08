#include "rapport.h"

void Rapport::definirpointDepartExtremite(Point point) {
    if (point == POINT_A || point == POINT_B) { pointDepartExtremite_ = point; }
}

void Rapport::definirpointDepartParcours(Point point) {
    if (point == POINT_C || point == POINT_D) { pointDepartParcours_ = point; }
}

void Rapport::definirOrientation(Orientation orientation) {
    orientation_ = orientation;
}

void Rapport::definirExtremiteTrouvee(Point nouvelleExtremite) {
    extremiteTrouvee_ = nouvelleExtremite;
}

const char* Rapport::pointEnString(Point point) {
    switch (point) {
        case POINT_A: return "A";
        case POINT_B: return "B";
        case POINT_C: return "C";
        case POINT_D: return "D";
        case POINT_E: return "E";
        default: return "Inconnu";
    }
}

const char* Rapport::orientationEnString(Orientation nouvelleOrientation) {
    switch (nouvelleOrientation) {
        case NORD_EST : return "NE";
        case NORD_OUEST : return "NO";
        case SUD_EST : return "SE";
        case SUD_OUEST : return "SO";
        default : return "Aucune";
    }
}

void Rapport::definirPoteau1(int position) {
    if (position >= 1 && position <= 2) {
        poteau1_ = position;
    }
}
void Rapport::definirPoteau2(int position) {
    if (position >= 3 && position <= 4) {
        poteau2_ = position;
    }
}
void Rapport::definirPoteau3(int position) {
    if (position >= 5 && position <= 6) {
        poteau3_ = position;
    }
}

void Rapport::envoyerRapport() {
    Usart::initialiser();
    Usart::transmettreDonne("\nIdentification de l'extremite\n");
    Usart::transmettreDonne("-----------------------------\n");
    Usart::transmettreDonne("Point de depart : ");
    Usart::transmettreDonne(pointEnString(pointDepartExtremite_));
    Usart::transmettreDonne("\nOrientation de depart : ");
    Usart::transmettreDonne(orientationEnString(orientation_));
    Usart::transmettreDonne("\nExtremite trouvee : ");
    Usart::transmettreDonne(pointEnString(extremiteTrouvee_));
    Usart::transmettreDonne("\n\n");

    Usart::transmettreDonne("Traversee du parcours\n");
    Usart::transmettreDonne("---------------------\n");
    Usart::transmettreDonne("Point de depart : ");
    Usart::transmettreDonne(pointEnString(pointDepartParcours_));
    Usart::transmettreDonne("\nPoint du poteau 1 : ");
    Usart::transmettreDonne(poteau1_);
    Usart::transmettreDonne("\nPoint du poteau 2 : ");
    Usart::transmettreDonne(poteau2_);
    Usart::transmettreDonne("\nPoint du poteau 3 : ");
    Usart::transmettreDonne(poteau3_);
    Usart::transmettreDonne("\n\n");

    Usart::transmettreDonne(numeroEquipe_);
    Usart::transmettreDonne(" - ");
    Usart::transmettreDonne(nomRobot_);
    Usart::transmettreDonne("\n");
}
