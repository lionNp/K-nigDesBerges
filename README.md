
# Durchführen eines Spiels mit nicht festgesetzter Iterationstiefe (dynamisches Zeitmanagement), initialer MCTS und Alpha-Beta suche mit transposition tables:
./compile.sh

## Anmerkung:
depth 1 ist bei uns Äquivalent zu einem Zug von A und einem Gegenzug von B, also der Tiefe 2 im Kontext des Kurses


# Training der Gewichte durch Spielen gegen sich selbst:
./compile.sh eval_weights_training

## Anmerkungen:
Bei zu wenig Speicher kann es zu Stacksmashing Fehlern kommen. In diesem Fall hilft erneutes ausführen manchmal :)
    

# Fitting der Weights mittels einigen Bewertungen auf Stellungen von Stockfish:
./compile.sh weights_fitting