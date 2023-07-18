
Durchführen eines Spiels mit nicht festgesetzter Iterationstiefe (dynamisches Zeitmanagement) und Alpha-Beta suche mit transposition tables:
./compile.sh


Training der Gewichte durch Spielen gegen sich selbst:
./compile.sh eval_weights_training

Anmerkungen:
    zu erwarten ist, dass Weiß die meisten Spiele zu anfang gewinnt (Siehe: MS4)
    für training mit "schlechteren" werten in "evaluation.c" Gewichte austauschen durch:
        float material_modify = 20;
        float position_modify = 1;
        float contol_modify =  2;
        float pawns_modify = 1;
        float king_safety_modify = 1;

    Bei zu wenig Speicher kann es zu Stacksmashing Fehlern kommen. In diesem Fall hilft erneutes ausführen manchmal :)
    