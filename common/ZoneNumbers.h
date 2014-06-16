#include "../common/types.h"

inline const char* StaticGetZoneName(uint32 zoneID) {
	// @merth: I did the following query to retrieve these (following by a simple find/replace)
	// select concat('case ', zoneidnumber), concat(short_name, '";') from zone order by zoneidnumber;
	switch (zoneID) {
		case 1: return "qeynos";
		case 2: return "qeynos2";
		case 3: return "qrg";
		case 4: return "qeytoqrg";
		case 5: return "highpass";
		case 6: return "highkeep";
		case 8: return "freportn";
		case 9: return "freportw";
		case 10: return "freporte";
		case 11: return "runnyeye";
		case 12: return "qey2hh1";
		case 13: return "northkarana";
		case 14: return "southkarana";
		case 15: return "eastkarana";
		case 16: return "beholder";
		case 17: return "blackburrow";
		case 18: return "paw";
		case 19: return "rivervale";
		case 20: return "kithicor";
		case 21: return "commons";
		case 22: return "ecommons";
		case 23: return "erudnint";
		case 24: return "erudnext";
		case 25: return "nektulos";
		case 26: return "cshome";
		case 27: return "lavastorm";
		case 28: return "nektropos";
		case 29: return "halas";
		case 30: return "everfrost";
		case 31: return "soldunga";
		case 32: return "soldungb";
		case 33: return "misty";
		case 34: return "nro";
		case 35: return "sro";
		case 36: return "befallen";
		case 37: return "oasis";
		case 38: return "tox";
		case 39: return "hole";
		case 40: return "neriaka";
		case 41: return "neriakb";
		case 42: return "neriakc";
		case 43: return "neriakd";
		case 44: return "najena";
		case 45: return "qcat";
		case 46: return "innothule";
		case 47: return "feerrott";
		case 48: return "cazicthule";
		case 49: return "oggok";
		case 50: return "rathemtn";
		case 51: return "lakerathe";
		case 52: return "grobb";
		case 53: return "aviak";
		case 54: return "gfaydark";
		case 55: return "akanon";
		case 56: return "steamfont";
		case 57: return "lfaydark";
		case 58: return "crushbone";
		case 59: return "mistmoore";
		case 60: return "kaladima";
		case 61: return "felwithea";
		case 62: return "felwitheb";
		case 63: return "unrest";
		case 64: return "kedge";
		case 65: return "guktop";
		case 66: return "gukbottom";
		case 67: return "kaladimb";
		case 68: return "butcher";
		case 69: return "oot";
		case 70: return "cauldron";
		case 71: return "airplane";
		case 72: return "fearplane";
		case 73: return "permafrost";
		case 74: return "kerraridge";
		case 75: return "paineel";
		case 76: return "hateplane";
		case 77: return "arena";
		case 78: return "fieldofbone";
		case 79: return "warslikswood";
		case 80: return "soltemple";
		case 81: return "droga";
		case 82: return "cabwest";
		case 83: return "swampofnohope";
		case 84: return "firiona";
		case 85: return "lakeofillomen";
		case 86: return "dreadlands";
		case 87: return "burningwood";
		case 88: return "kaesora";
		case 89: return "sebilis";
		case 90: return "citymist";
		case 91: return "skyfire";
		case 92: return "frontiermtns";
		case 93: return "overthere";
		case 94: return "emeraldjungle";
		case 95: return "trakanon";
		case 96: return "timorous";
		case 97: return "kurn";
		case 98: return "erudsxing";
		case 100: return "stonebrunt";
		case 101: return "warrens";
		case 102: return "karnor";
		case 103: return "chardok";
		case 104: return "dalnir";
		case 105: return "charasis";
		case 106: return "cabeast";
		case 107: return "nurga";
		case 108: return "veeshan";
		case 109: return "veksar";
		case 110: return "iceclad";
		case 111: return "frozenshadow";
		case 112: return "velketor";
		case 113: return "kael";
		case 114: return "skyshrine";
		case 115: return "thurgadina";
		case 116: return "eastwastes";
		case 117: return "cobaltscar";
		case 118: return "greatdivide";
		case 119: return "wakening";
		case 120: return "westwastes";
		case 121: return "crystal";
		case 123: return "necropolis";
		case 124: return "templeveeshan";
		case 125: return "sirens";
		case 126: return "mischiefplane";
		case 127: return "growthplane";
		case 128: return "sleeper";
		case 129: return "thurgadinb";
		case 130: return "erudsxing2";
		case 150: return "shadowhaven";
		case 151: return "bazaar";
		case 152: return "nexus";
		case 153: return "echo";
		case 154: return "acrylia";
		case 155: return "sharvahl";
		case 156: return "paludal";
		case 157: return "fungusgrove";
		case 158: return "vexthal";
		case 159: return "sseru";
		case 160: return "katta";
		case 161: return "netherbian";
		case 162: return "ssratemple";
		case 163: return "griegsend";
		case 164: return "thedeep";
		case 165: return "shadeweaver";
		case 166: return "hollowshade";
		case 167: return "grimling";
		case 168: return "mseru";
		case 169: return "letalis";
		case 170: return "twilight";
		case 171: return "thegrey";
		case 172: return "tenebrous";
		case 173: return "maiden";
		case 174: return "dawnshroud";
		case 175: return "scarlet";
		case 176: return "umbral";
		case 179: return "akheva";
		case 180: return "arena2";
		case 181: return "jaggedpine";
		case 182: return "nedaria";
		case 183: return "tutorial";
		case 184: return "load";
		case 185: return "load2";
		case 186: return "hateplaneb";
		case 187: return "shadowrest";
		case 188: return "tutoriala";
		case 189: return "tutorialb";
		case 190: return "clz";
		case 200: return "codecay";
		case 201: return "pojustice";
		case 202: return "poknowledge";
		case 203: return "potranquility";
		case 204: return "ponightmare";
		case 205: return "podisease";
		case 206: return "poinnovation";
		case 207: return "potorment";
		case 208: return "povalor";
		case 209: return "bothunder";
		case 210: return "postorms";
		case 211: return "hohonora";
		case 212: return "solrotower";
		case 213: return "powar";
		case 214: return "potactics";
		case 215: return "poair";
		case 216: return "powater";
		case 217: return "pofire";
		case 218: return "poeartha";
		case 219: return "potimea";
		case 220: return "hohonorb";
		case 221: return "nightmareb";
		case 222: return "poearthb";
		case 223: return "potimeb";
		case 224: return "gunthak";
		case 225: return "dulak";
		case 226: return "torgiran";
		case 227: return "nadox";
		case 228: return "hatesfury";
	}
	return "UNKNWN";
}
