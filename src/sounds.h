#include "pitches.h"
const long FOURTH = 1000 / 4;
const long EIGHT = 1000 / 8;
const long TWELWETH = 1000 / 12;
const long SIXTH = 1000 / 6;
const long TWOTH = 1000 / 2;
const long SEVENTH = 7.5 / 1000;

int LOGIN_SOUND[4][2] = {
	{ NOTE_C4, 200 },
	{ NOTE_D4, 200 },
	{ NOTE_E4, 200 }
};

int NOTIFICATION_SOUND[2][2] = {
		{ NOTE_E4, 100 }
};

int LOGOUT_SOUND[3][2] = {
	{ NOTE_E4, 400 },
	{ NOTE_D3, 100 }
};

int LID_OPENED[4][2] = {
	{ NOTE_E5, 100 },
	{ -2, 50 },
	{ NOTE_E5, 100 }
};

int CONNECTION_ESTABLISHED[6][2] = {
	{ NOTE_C4, 200 },
	{ NOTE_D4, 200 },
	{ NOTE_E4, 200 },
	{ NOTE_D4, 200 },
	{ NOTE_C4, 200 }
};

int CONNECTION_CLOSED[6][2] = {
	{ NOTE_E5, 500 },
	{ -2, 50 },
	{ NOTE_E5, 100 },
	{ -2, 50 },
	{ NOTE_E5, 500 }
};

int SUPER_MARIO_SONG[150][2] = {
	{ NOTE_E7, EIGHT },
	{ NOTE_E7, EIGHT },
	{ -2, EIGHT },
	{ NOTE_E7, EIGHT },
	{ -2, EIGHT },
	{ NOTE_C7, EIGHT },
	{ NOTE_E7, EIGHT },
	{ -2, EIGHT },
	{ NOTE_G7, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },
	{ NOTE_G6, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },

	{ NOTE_C7, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },
	{ NOTE_G6, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },
	{ NOTE_E6, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },
	{ NOTE_A6, EIGHT },
	{ -2, EIGHT },
	{ NOTE_B6, EIGHT },
	{ -2, EIGHT },
	{ NOTE_AS6, EIGHT },
	{ NOTE_A6, EIGHT },
	{ -2, EIGHT },

	{ NOTE_G6, SIXTH },
	{ NOTE_E7, SIXTH },
	{ NOTE_G7, SIXTH },
	{ NOTE_A7, EIGHT },
	{ -2, EIGHT },
	{ NOTE_F7, EIGHT },
	{ NOTE_G7, EIGHT },
	{ -2, EIGHT },
	{ NOTE_E7, EIGHT },
	{ -2, EIGHT },
	{ NOTE_C7, EIGHT },
	{ NOTE_D7, EIGHT },
	{ NOTE_B6, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },

	{ NOTE_C7, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },
	{ NOTE_G6, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },
	{ NOTE_E6, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },
	{ NOTE_A6, EIGHT },
	{ -2, EIGHT },
	{ NOTE_B6, EIGHT },
	{ -2, EIGHT },
	{ NOTE_AS6, EIGHT },
	{ NOTE_A6, EIGHT },
	{ -2, EIGHT },

	{ NOTE_G6, SIXTH },
	{ NOTE_E7, SIXTH },
	{ NOTE_G7, SIXTH },
	{ NOTE_A7, EIGHT },
	{ -2, EIGHT },
	{ NOTE_F7, EIGHT },
	{ NOTE_G7, EIGHT },
	{ -2, EIGHT },
	{ NOTE_E7, EIGHT },
	{ -2, EIGHT },
	{ NOTE_C7, EIGHT },
	{ NOTE_D7, EIGHT },
	{ NOTE_B6, EIGHT },
	{ -2, EIGHT },
	{ -2, EIGHT },

	// underworld
	{ NOTE_C4, EIGHT },
	{ NOTE_C5, EIGHT },
	{ NOTE_A3, EIGHT },
	{ NOTE_A4, EIGHT },
	{ NOTE_AS3, EIGHT },
	{ NOTE_AS4, EIGHT },
	{ -2, FOURTH },
	{ -2, TWOTH },

	{ NOTE_C4, EIGHT },
	{ NOTE_C5, EIGHT },
	{ NOTE_A3, EIGHT },
	{ NOTE_A4, EIGHT },
	{ NOTE_AS3, EIGHT },
	{ NOTE_AS4, EIGHT },
	{ -2, FOURTH },
	{ -2, TWOTH },

	{ NOTE_F3, EIGHT },
	{ NOTE_F4, EIGHT },
	{ NOTE_D3, EIGHT },
	{ NOTE_D4, EIGHT },
	{ NOTE_DS3, EIGHT },
	{ NOTE_DS4, EIGHT },
	{ -2, FOURTH },
	{ -2, TWOTH },

	{ NOTE_F3, EIGHT },
	{ NOTE_F4, EIGHT },
	{ NOTE_D3, EIGHT },
	{ NOTE_D4, EIGHT },
	{ NOTE_DS3, EIGHT },
	{ NOTE_DS4, EIGHT },
	{ -2, FOURTH },
	{ -2, FOURTH },

	{ NOTE_DS4, TWELWETH },
	{ NOTE_CS4, TWELWETH },
	{ NOTE_D4, TWELWETH },
	{ NOTE_CS4, FOURTH },
	{ NOTE_DS4, FOURTH },
	{ NOTE_DS4, FOURTH },
	{ NOTE_GS3, FOURTH },
	{ NOTE_G3, FOURTH },
	{ NOTE_CS4, FOURTH },
	{ NOTE_C4, TWELWETH },
	{ NOTE_FS4, TWELWETH },
	{ NOTE_F4, TWELWETH },
	{ NOTE_E3, TWELWETH },
	{ NOTE_AS4, TWELWETH },
	{ NOTE_A4, TWELWETH },
	{ NOTE_GS4, SEVENTH },
	{ NOTE_DS4, SEVENTH },
	{ NOTE_B3, SEVENTH },
	{ NOTE_AS3, SEVENTH },
	{ NOTE_A3, SEVENTH },
	{ NOTE_GS3, SEVENTH },
	{ -2, TWOTH },
	{ -2, TWOTH },
	{ -2, TWOTH }

};
