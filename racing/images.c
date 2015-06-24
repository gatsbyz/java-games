//

#include "images.h"

const u16* apple_img[24] = { mc, mc_r, pc1, pc2, rc1, rc1, rc1, rc1, rc2, rc2, rc2, rc2, rc3, rc3, rc3, rc3, rc4, rc4, rc4, rc4, rc5, rc5, rc5, rc5 };

SIZE getSize(int n)
{
	SIZE s;
	switch (n) {
	case 0:
		s.width = MC_WIDTH;
		s.height = MC_HEIGHT;
		break;
	case 1:
		s.width = MC_R_WIDTH;
		s.height = MC_R_HEIGHT;
		break;
	case 2:
		s.width = PC1_WIDTH;
		s.height = PC1_HEIGHT;
		break;
	case 3:
		s.width = PC2_WIDTH;
		s.height = PC2_HEIGHT;
		break;
	case 4:
		s.width = RC1_WIDTH;
		s.height = RC1_HEIGHT;
		break;
	case 5:
		s.width = RC1_WIDTH;
		s.height = RC1_HEIGHT;
		break;
	case 6:
		s.width = RC1_WIDTH;
		s.height = RC1_HEIGHT;
		break;
	case 7:
		s.width = RC2_WIDTH;
		s.height = RC2_HEIGHT;
		break;
	case 8:
		s.width = RC2_WIDTH;
		s.height = RC2_HEIGHT;
		break;
	case 9:
		s.width = RC2_WIDTH;
		s.height = RC2_HEIGHT;
		break;
	case 10:
		s.width = RC2_WIDTH;
		s.height = RC2_HEIGHT;
		break;
	case 11:
		s.width = RC2_WIDTH;
		s.height = RC2_HEIGHT;
		break;
	case 12:
		s.width = RC3_WIDTH;
		s.height = RC3_HEIGHT;
		break;
	case 13:
		s.width = RC3_WIDTH;
		s.height = RC3_HEIGHT;
		break;
	case 14:
		s.width = RC3_WIDTH;
		s.height = RC3_HEIGHT;
		break;
	case 15:
		s.width = RC3_WIDTH;
		s.height = RC3_HEIGHT;
		break;
	case 16:
		s.width = RC4_WIDTH;
		s.height = RC4_HEIGHT;
		break;
	case 17:
		s.width = RC4_WIDTH;
		s.height = RC4_HEIGHT;
		break;
	case 18:
		s.width = RC4_WIDTH;
		s.height = RC4_HEIGHT;
		break;
	case 19:
		s.width = RC4_WIDTH;
		s.height = RC4_HEIGHT;
		break;
	case 20:
		s.width = RC5_WIDTH;
		s.height = RC5_HEIGHT;
		break;
	case 21:
		s.width = RC5_WIDTH;
		s.height = RC5_HEIGHT;
		break;
	case 22:
		s.width = RC5_WIDTH;
		s.height = RC5_HEIGHT;
		break;
	case 23:
		s.width = RC5_WIDTH;
		s.height = RC5_HEIGHT;
		break;
	default:
		s.width = -1;
		s.height = -1;
	}
	return s;
}

