#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp_littlefs.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"
#include "flatbuffers/flatbuffers.h"

#define SAMPLE_MELODY_FILE "/data/sample_melody.fbs"

struct Melody {
  char *label;
  float speed;
  struct Tone tones[10];
};

struct Tone {
  float frequency;
  int duration;
};

static void GenerateRandomTone() {
  struct Melody melody = {
    .label = "My Melody",
    .speed = 1.0f,
    .tones = {
      {
        .frequency = 440.0f,
        .duration = 1000,
      },
      {
        .frequency = 880.0f,
        .duration = 1000,
      },
      {
        .frequency = 1760.0f,
        .duration = 1000,
      },
    },
  };

  int ret = esp_littlefs_write(SAMPLE_MELODY_FILE, &melody, sizeof(melody));
  if (ret != ESP_OK) {
    ESP_LOGE("Failed to write melody to LittleFS: %d", ret);
    return;
  }

  ESP_LOGI("Successfully wrote melody to LittleFS");
}

static void PlayMelody(const char *filename) {
  struct Melody melody;

  int ret = esp_littlefs_read(filename, &melody, sizeof(melody));
  if (ret != ESP_OK) {
    ESP_LOGE("Failed to read melody from LittleFS: %d", ret);
    return;
  }

  ESP_LOGI("Playing melody: '%s'", melody.label);

  for (int i = 0; i < ARRAY_SIZE(melody.tones); i++) {
    ESP_LOGI("  Tone %d: frequency=%.2f, duration=%d", i, melody.tones[i].frequency, melody.tones[i].duration);
  }
}

void app_main(void) {
  GenerateRandomTone();
  PlayMelody(SAMPLE_MELODY_FILE);
}
