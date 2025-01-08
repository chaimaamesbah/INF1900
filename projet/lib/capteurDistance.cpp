#include "capteurDistance.h"

CapteurDistance::CapteurDistance(can *can, Del *del) : can_(can), del_(del) {
    DDRA &= ~(1 << pinCapteur_);
    estPoteauDetecte_ = false;
}

uint8_t CapteurDistance::obtenirFrequence() {
    frequenceLectureCapteur_ = can_->lecture(pinCapteur_);
    return frequenceLectureCapteur_;
}

void CapteurDistance::afficherEtGererLeds() {
    frequenceLectureCapteur_ = can_->lecture(pinCapteur_);
    sprintf(buffer, "%u", frequenceLectureCapteur_);
    Usart::transmettreDonne("Valeur du capteur : ");
    Usart::transmettreDonne(buffer);
    Usart::transmettreDonne("\n");
}

bool CapteurDistance::estPoteauDetecteDixCm() {
    frequenceLectureCapteur_ = can_->lecture(pinCapteur_);
    estPoteauDetecte_ = (frequenceLectureCapteur_ >= frequencePoteauDixCmMinimum_ &&
                        frequenceLectureCapteur_ <= frequencePoteauDixCmMaximum_);
    return estPoteauDetecte_;
}

bool CapteurDistance::estPoteauDetecteTrenteCm() {
    frequenceLectureCapteur_ = can_->lecture(pinCapteur_);
    estPoteauDetecte_ = (frequenceLectureCapteur_ >= frequencePoteauTrenteCmMaximum_);
    return estPoteauDetecte_;
}