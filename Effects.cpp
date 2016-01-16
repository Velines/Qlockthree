/**
* Effects.cpp
* Klasse für diverse Effekte
*
* @mc       Arduino/UNO
* @autor    Manuel Bracher / manuel.bracher@gmail.com
* @version  1.0
* @created  02.01.15
*
* Versionshistorie:
* V 1.0:  - Erstellt.
*/

#include "Effects.h"
#include "Staben.h"

// #define DEBUG
#include "Debug.h"
      
void Effects::showTickerString(const char* str2disp, byte tickerSpeed) {
  word matrix [16];

  int strLength = strlen(str2disp);//((String)str2disp).length();//sizeof(str2disp) / sizeof(char*);
  long bufLen;
  char actChar;
  char lastChar = 'W';
  int t = 2;
  boolean finish = false;
  int i = 0;

  while (!finish) {
    _renderer->clearScreenBuffer(matrix);
    unsigned int shift = 0; // Schiebekorrektur aufgrund variierender Buchstabenbreite
    for (byte k = 0; k < strLength; k++) {
      actChar = str2disp[k];
      if (actChar == ' ') {
        shift += 3;  //bei einem Space eine Lücke von:
      }
      else {
        shift -= pgm_read_byte_near(&(stabenBig[lastChar - '!'][7]));
        for (byte j = 0; j < 7; j++) {
          if (!(actChar == ' ')) {
            matrix[t + j] |= (pgm_read_byte_near(&(stabenBig[actChar - '!'][j])) << (1 - shift + i)) & 0b1111111111100000;
          }
        }
        if (k < (strLength - 1)) {
          shift += 6; // Max. Buchstabenbreite + ein Pixel Lücke
        }
        lastChar = actChar;
      }
    }
    for (int k = 0; k < 1 + 3 * (10 - tickerSpeed); k++) {
      _ledDriver->writeScreenBufferToMatrix(matrix, true);
    }
    bufLen = shift + 15;
    if (i == bufLen) {
      finish = true;
    }
    else {
      i++;
    }
  }
}

///**
// * Intro
// */
//void Effects::showIntro() {
//  word matrix [16];
//
//  _renderer->clearScreenBuffer(matrix);
//  for (int j = 0; j < 11; j++) {
//    for (byte i = 0; i < 10; i++) {
//      matrix[i] |= 0b1 << 15 - j;
//    }
//    for (int k = 0; k < 5; k++) {
//      _ledDriver->writeScreenBufferToMatrix(matrix, true);
//    }
//  }
//  for (int j = 0; j < 11; j++) {
//    for (int i = 0; i < 10; i++) {
//      matrix[i] ^= 0b1 << 5 + j;
//    }
//    for (int k = 0; k < 5; k++) {
//      _ledDriver->writeScreenBufferToMatrix(matrix, true);
//    }
//  }
//  _renderer->clearScreenBuffer(matrix);
//  for (int i = 9; i >= 0; i--) {
//    matrix[i] |= 0b1111111111100000;
//    for (int k = 0; k < 5; k++) {
//      _ledDriver->writeScreenBufferToMatrix(matrix, true);
//    }
//  }
//  for (int i = 0; i < 10; i++) {
//    matrix[i] ^= 0b1111111111100000;
//    for (int k = 0; k < 5; k++) {
//      _ledDriver->writeScreenBufferToMatrix(matrix, true);
//    }
//  }
//}

/**
 * Pulsierender Herz-Effekt
 */
void Effects::showHeart() {
  word matrix [16];
  for ( int y = 0; y < 3; y++) {
    _renderer->clearScreenBuffer(matrix);
    for (int j = 0; j < 8; j++) {
      matrix[1 + j] |= (pgm_read_word_near(&(effectMasks[0][j])) << 5);
    }
    for (int k = 0; k < 40; k++) {
      _ledDriver->writeScreenBufferToMatrix(matrix, true);
    }
    for ( int i = 0; i < 2; i++) {
      _renderer->clearScreenBuffer(matrix);
      for (int z = 0; z < 2; z++) {
        for (int j = 0; j < 8; j++) {
          matrix[1 + j] |= (pgm_read_word_near(&(effectMasks[z][j])) << 5);
        }
        for (int k = 0; k < 10; k++) {
          _ledDriver->writeScreenBufferToMatrix(matrix, true);
        }
      }
    }
  }
  _renderer->clearScreenBuffer(matrix);
  for (int j = 0; j < 8; j++) {
    matrix[1 + j] |= (pgm_read_word_near(&(effectMasks[0][j])) << 5);
  }
  for (int k = 0; k < 35; k++) {
    _ledDriver->writeScreenBufferToMatrix(matrix, true);
  }
}

/**
 * Feuerwerk-Effekt
 */
void Effects::showFireWork(byte posX) {
  word matrix [16];

  for (int i = 9; i >= 3; i--) {
    _renderer->clearScreenBuffer(matrix);
    _ledDriver->setPixelInScreenBuffer(posX, i, matrix);
    for (int k = 0; k < 7; k++) {
      _ledDriver->writeScreenBufferToMatrix(matrix, true);
    }
  }

  for (int i = 8; i <= 10; i++) {
    _renderer->clearScreenBuffer(matrix);
    for (int j = 0; j < 10; j++) {
      matrix[j] |= (pgm_read_word_near(&(effectMasks[i][j])) << 10 - posX) & 0b1111111111100000;
    }
    for (int k = 0; k < (3 + round(10 * (i - 8) / 3)); k++) {
      _ledDriver->writeScreenBufferToMatrix(matrix, true);
    }
  }
  for (int i = 0; i <= 10; i++) {
    _renderer->clearScreenBuffer(matrix);
    for (int j = 0; j < 10 - i; j++) {
      matrix[j + i] |= (pgm_read_word_near(&(effectMasks[12 + i % 2][j])) << 10 - posX) & 0b1111111111100000;
    }
    for (int k = 0; k < 10; k++) {
      _ledDriver->writeScreenBufferToMatrix(matrix, true);
    }
  }
}

/**
 * Kerzen-Effekt
 */
void Effects::showCandle() {
  word matrix [16];
  for (int k = 0; k < 5; k++) {
    for (int j = -4; j < 4; j++) {
      _renderer->clearScreenBuffer(matrix);
      for (int i = 5; i < 10; i++) {
        matrix[i] |= (pgm_read_word_near(&(effectMasks[7][i])) << 5);
      }
      for (int i = 0; i < 5; i++) {
        matrix[i] |= (pgm_read_word_near(&(effectMasks[2 + 4 - abs(j % 4)][i])) << 5);
      }
      for (int i = 0; i < 10; i++) {
        _ledDriver->writeScreenBufferToMatrix(matrix, true);
      }
    }
  }
}

///**
// * Love U
// */
//void Effects::showLoveU() {
//  word matrix [16];
//  _renderer->clearScreenBuffer(matrix);
//  for (int i = 0; i < 10; i++) {
//    matrix[i] |= (pgm_read_word_near(&(effectMasks[14][i])) << 5);
//  }
//  for (int i = 0; i < 400; i++) {
//    _ledDriver->writeScreenBufferToMatrix(matrix, true);
//  }
//}

/**
 * Bitmap
 */
void Effects::showBitmap(byte bitmapIdx, byte duration) {
  word matrix [16];
  _renderer->clearScreenBuffer(matrix);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 11; j++) {
      matrix[i] |= ((pgm_read_word_near(&(bitmaps[bitmapIdx - BITMAP_MIN][j])) >> i) & 0x0001) << 15 - j;
    }
  }
  for (int i = 0; i < duration * 50; i++) {
    _ledDriver->writeScreenBufferToMatrix(matrix, true);
  }
}

/**
 * Bitmap-Effekt
 */
void Effects::showAnimatedBitmap(byte animatedBitmap) {
  switch (animatedBitmap) {
    case ANI_BITMAP_CHAMPGLASS:
      for (int i = 0; i < 6; i++) {
        showBitmap(BITMAP_CHAMPGLASS1 + i % 2, 1);
      }
      break;
    case ANI_BITMAP_CHRISTTREE:
      for (int i = 0; i < 4; i++) {
        showBitmap(BITMAP_CHRISTTREE1 + i % 2, 1);
      }
      break;
    case ANI_BITMAP_SMILEY_WINK:
      showBitmap(BITMAP_SMILEY, 1);
      showBitmap(BITMAP_SMILEY_WINK, 1);
      showBitmap(BITMAP_SMILEY, 1);
      break;
  }
}


