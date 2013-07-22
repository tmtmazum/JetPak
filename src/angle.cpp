#include "../headers/angle.h"
#include "../headers/Pos.h"

anglePI::anglePI(const anglePI& an) { value = an.value; }
anglePI::anglePI(const angleRAD& an) { value = an.value / PI; }
anglePI::anglePI(const angleDEG& an) { value = an.value / 180; }

angleRAD::angleRAD(const angleRAD& an) { value = an.value; }
angleRAD::angleRAD(const angleDEG& an) { value = an.value * (PI / 180); }
angleRAD::angleRAD(const anglePI& an) { value = an.value * PI; }

angleDEG::angleDEG(const angleDEG& an) { value = an.value; }
angleDEG::angleDEG(const angleRAD& an) { value = an.value * (180 / PI); }
