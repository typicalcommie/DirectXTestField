#pragma once

#define _DELETE(x) {if(x) delete x; x = 0;}
#define _MIDELETE(x) {if(x) x->Release(); x = 0;}
#define _HRISOK(x) {if(FAILED(x)) return false;}