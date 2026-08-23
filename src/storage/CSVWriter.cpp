#include <Arduino.h>
#include <FS.h>
#include <SD.h>

#include "config.h"
#include "types.h"

void csvWriteStatistics(const StatisticsData& s) {
    File file = SD.open(STATS_FILE, FILE_APPEND);

    if (!file) {
        return;
    }

    // Cria o cabeçalho se o arquivo estiver vazio.
    if (file.size() == 0) {
        file.println(
            "uptime,frames,management,control,data,eapol,aps,clients,channel,rssi"
        );
    }

    file.printf(
        "%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%u,%d\n",
        (unsigned long)s.uptime,
        (unsigned long)s.frames,
        (unsigned long)s.management,
        (unsigned long)s.control,
        (unsigned long)s.data,
        (unsigned long)s.eapol,
        (unsigned long)s.aps,
        (unsigned long)s.clients,
        s.channel,
        s.rssi
    );

    file.close();
}