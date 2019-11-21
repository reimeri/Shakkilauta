const int BOARD_WIDTH = 2;
const int BOARD_HEIGTH = 2;

const char ROW[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
const char COLUMN[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

bool old_board_cells[8][8];
bool board_cells[8][8];

int turn_pass_button = 20;
int turn_pass_button_val = LOW;
int turn_pass_button_val_old = LOW;

void setup() {
  // Nappi jolla vuoro päätetään
  pinMode(turn_pass_button, INPUT);

  // Asetetaan pinnit INPUT modeen
  for (int i = 0; i < BOARD_WIDTH + BOARD_HEIGTH; i++) {
    pinMode(i, INPUT);
  }
  Serial.begin(9600);
}

void loop() {
  readButtons();
  
  // Kun turn_pass_buttonia painetaan luetaan pinnit ja verrataan niitä
  // aikaisempaan laudan tilaan
  if (turn_pass_button_val) {
    // Tallennetaan edellinen laudan tila toiseen muuttujaan
    for (int i = 0; i < BOARD_WIDTH; i++) {
      for (int j = 0; j <  BOARD_HEIGTH; j++) {
        old_board_cells[i][j] = board_cells[i][j];
      }
    }

    readPins();
    String move_uci = compareBoard();
  }
}

// 8 ensimmäistä pinniä on x-akseli ja loput 8 on y-akseli
// Luetaan pinnit jonka jälkeen katsotaan onko niiden antamat arvot 1
// jos on asetetaan board_cells arrayn arvo trueksi
void readPins() {
  for (int i = 0; i < BOARD_WIDTH; i++) {
    for (int j = 0; j <  BOARD_HEIGTH; j++) {
      int pin_x = digitalRead(i);
      int pin_y = digitalRead(j + 8);
      if (pin_x == 1 && pin_y == 1) {
        board_cells[i][j] = true;
      } else {
        board_cells[i][j] = false;
      }
    }
  }
}

// Verrataan aikaisempaa laudan tilaa nykyiseen
// ja palautetaan siirto UCI muodossa
String compareBoard() {
  // Move written in UCI standard
  String move_uci = "";
  String move_start = "";
  String move_end = "";
  for (int i = 0; i < BOARD_WIDTH; i++) {
    for (int j = 0; j <  BOARD_HEIGTH; j++) {
      // Jos ruudun tila on muuttunut tiedetään että nappula on joko siirtynyt
      // ruutuun tai ruudusta on siirretty nappula pois
      if(board_cells[i][j] != old_board_cells[i][j]){
        // Jos edellisessä laudan tilassa ruudun tila oli true niin silloin sen
        // täytyi olla ruutu josta nappula siirtyi pois
        if(old_board_cells[i][j] == true){
          move_start = ROW[i] + COLUMN[j];
        }else{
          move_end = ROW[i] + COLUMN[j];
        }
      }
    }
  }
  move_uci = move_start + move_end;
  return move_uci;
}

// Luetaan eri nappuloiden tilat ei varmasti ole paras tapa tehdä tätä
// mutta tuon pitäisi estää useat painallukset nappia pohjassa pidettäessä
void readButtons() {
  int tmp_val1 = digitalRead(turn_pass_button);
  
  if(tmp_val1 != turn_pass_button_val_old){
    turn_pass_button_val = tmp_val1;
  }else{
    turn_pass_button_val = LOW;
  }

  turn_pass_button_val_old = tmp_val1;
}
