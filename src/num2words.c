#include "num2words.h"
#include "string.h"

static const char* const STUNDEN[] = {
  "zwölf",
  "eins",
  "zwei",
  "drei",
  "vier",
  "fünf",
  "sechs",
  "sieben",
  "acht",
  "neun",
  "zehn",
  "elf"
};

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}

void fuzzy_time_to_words(int hours, int minutes, char* words1, char* words2, char* words3, char* words4, size_t length) {
  
  
  size_t remaining1 = length;
  size_t remaining2 = length;
  size_t remaining3 = length;
  size_t remaining4 = length;
  memset(words1, 0, length);
	memset(words2,0,length);
	memset(words3,0,length);
	memset(words4,0,length);
  
  if (minutes == 0) {
    if (hours == 0) {
      remaining3 -= append_string(words3, remaining3, "mitternacht");
    } else {
      if (hours % 12 == 1) {
        remaining2 -= append_string(words2, remaining2, "ein");
      } else {
        remaining2 -= append_string(words2, remaining2, STUNDEN[hours % 12]);
      }
      remaining3 -= append_string(words3, remaining3, "uhr");
      if (hours  < 5 || hours > 21) {
        remaining4 -= append_string(words4, remaining4, "nachts");
      } else if ( hours < 12) {
        remaining4 -= append_string(words4, remaining4, "morgens");
      } else if ( hours < 13) {
        remaining4 -= append_string(words4, remaining4, "mittags");
      } else if ( hours < 18) {
        remaining4 -= append_string(words4, remaining4, "nachm.");
      } else {
        remaining4 -= append_string(words4, remaining4, "abends");
      }
    }
    return;
  };
  
  if (minutes <= 3) {
    remaining2 -= append_string(words2, remaining2, "kurz");
	remaining2 -= append_string(words3,remaining3,"nach");
  } else if (minutes <= 8) {
    remaining2 -= append_string(words2, remaining2, "fünf");
	remaining3 -= append_string(words3,remaining3,"nach");
  } else if (minutes <= 12) {
    remaining2 -= append_string(words2, remaining2, "zehn");
	remaining3 -= append_string(words3,remaining3,"nach");
  } else if (minutes <= 17) {
    remaining2 -= append_string(words2, remaining2, "viertel");
	remaining3 -= append_string(words3,remaining3,"nach");
  } else if (minutes <= 22) {
    remaining2 -= append_string(words2, remaining2, "zwanzig");
	remaining3 -= append_string(words3,remaining3,"nach");
  } else if (minutes <= 28) {
    remaining1 -= append_string(words1, remaining1, "fünf"); 
	remaining2 -= append_string(words2,remaining2,"vor");
	remaining3 -= append_string(words3,remaining3,"halb");
  } else if (minutes <= 29) {
    remaining1 -= append_string(words1, remaining1, "kurz");
	remaining2 -= append_string(words2,remaining2,"vor");
	remaining3 -= append_string(words3,remaining3,"halb");
  } else if (minutes <= 30) {
    remaining3 -= append_string(words3, remaining3, "halb");
  } else if (minutes <= 32) {
    remaining1 -= append_string(words1, remaining1, "kurz");
	remaining2 -= append_string(words2, remaining2, "nach");
	remaining3 -= append_string(words3, remaining3, "halb");
  } else if (minutes <= 37) {
    remaining1 -= append_string(words1, remaining1, "fünf");
	remaining2 -= append_string(words2, remaining2, "nach");
	remaining3 -= append_string(words3, remaining3, "halb");
  } else if (minutes <= 43) {
    remaining2 -= append_string(words2, remaining2, "zwanzig");
	remaining3 -= append_string(words3, remaining3, "vor");
  } else if (minutes <= 48) {
    remaining2 -= append_string(words2, remaining2, "viertel");
	remaining3 -= append_string(words3, remaining3, "vor");
  } else if (minutes <= 53) {
    remaining2 -= append_string(words2, remaining2, "zehn");
	remaining3 -= append_string(words3, remaining3, "vor");
  } else if (minutes <= 57) {
    remaining2 -= append_string(words2, remaining2, "fünf");
	remaining3 -= append_string(words3, remaining3, "vor");
  } else if (minutes <= 59) {
    remaining2 -= append_string(words2, remaining2, "kurz");
	remaining3 -= append_string(words3, remaining3, "vor");
  }
  
  if (minutes < 23) {
    remaining4 -= append_string(words4, remaining4, STUNDEN[hours % 12]);
  } else {
    remaining4 -= append_string(words4, remaining4, STUNDEN[(hours+1) % 12]);
  }
}
