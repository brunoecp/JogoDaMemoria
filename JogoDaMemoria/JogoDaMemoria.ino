 String recebido;

String ordemBotaos;

String sequenciaNumerica = "";

String modo;

const int LED_PINS[] = {8, 9, 10,11}; // Array de LEDs
const int BUTTONS[] = {2,3,4,5};
const int NUM_BUTTONS = sizeof(BUTTONS)/ sizeof(BUTTONS[0]);
const int buzzer = 6;
const long SERIAL_TIMEOUT = 10000; // Timeout de 10 segundos para leitura do Serial
const int NUM_LEDS = sizeof(LED_PINS)/ sizeof(LED_PINS[0]);

long numeroGerado;

int nivel = 1;

int sequencia;

int seq_pontuacao = 0;

String Dificuldade;

// Definindo o enum para os estados do jogo
enum GameState {
  START_GAME,
  PLAY,
  GAME_OVER,
  PLAY_AGAIN
};
GameState stateGame = START_GAME;

void setup() {
  // Iniciar comunicacao serial
  Serial.begin(9600);

  // Definir LEDs como OUTPUTs pinos 8,9 e 10
  for(int i = 0; i < NUM_LEDS; i++){
    pinMode(LED_PINS[i], OUTPUT);
  for(int i = 0; i< NUM_BUTTONS; i++){
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }
  pinMode(buzzer, OUTPUT);
  }
  // Definir a seed do gerador de números aleatórios
  randomSeed(analogRead(0));

}

void loop() {
  switch (stateGame) {
    case START_GAME:
      Serial.println("* INICIO *");
      Serial.println("Comecar? (s/n)");
      leserial();
      escolherModo();
      if (recebido.equalsIgnoreCase("s")){
        ChooseDif();
        stateGame = PLAY;
        Serial.println("Jogo começando...");
        piscaled(1000,3);
      } else {
        stateGame = GAME_OVER;
        Serial.println("Jogo não iniciado"); 
        piscaled(300,5);     
      }
      break;

    case PLAY:
     jogo();
        stateGame= GAME_OVER;

    case GAME_OVER:
      Serial.println("Game Over"); 
      piscaled(100,5);
      stateGame = PLAY_AGAIN;
      break;

    case PLAY_AGAIN:
      Serial.println("jogar novamente? (s/n)"); 
      leserial();
      if (recebido.equalsIgnoreCase("s")){
        stateGame = PLAY;
        Serial.println("Jogo comecando...");
        piscaled(1000,3);
      } else {
        stateGame = GAME_OVER;
        Serial.println("Jogo nao iniciado"); 
        piscaled(300,5);     
      }
      break;
  }
}

void leserial(){

  Serial.println("* Insera sua resposta *");

  // Aguardar a resposta do usuario pelo tempo definido em SERIAL_TIMEOUT
  long startTime = millis();
  while (Serial.available() == 0 && millis() - startTime < SERIAL_TIMEOUT)
  {}

  // guarda o valor digitado pelo usuario em recebido
  if (Serial.available()) {
    recebido = Serial.readString();
  } else {
    Serial.println("Timeout! Resposta não recebida.");
  }
}

void piscaled(int tempo, int vezes){
  for(int i = 0; i < vezes; i++){
    for(int j = 0; j < NUM_LEDS; j++){
      digitalWrite(LED_PINS[j], HIGH);
    }
    delay(tempo);
    for(int j = 0; j < NUM_LEDS; j++){
      digitalWrite(LED_PINS[j], LOW);
    }
    delay(tempo);
  }
}

void geraSequencia (int tempo, int sequencia){
  // Criar uma lista de inteiros com o tamanho que e passado como argumento
  int ordemLeds[sequencia];

  // Gerar sequencia aleatoria
  for (int i = 0; i < sequencia; i++){
    ordemLeds[i] = random(1, NUM_LEDS + 1);
  }

  // Inicialmente, a String sequenciaNumerica é uma String vazia
  sequenciaNumerica = "";

  // Pisca os LEDs na sequencia gerada
  for (int j = 0; j < sequencia; j++){
    int ledIndex = ordemLeds[j] - 1;
    digitalWrite(LED_PINS[ledIndex], HIGH);
    buzina(j, tempo - 500);
    delay(tempo);
    digitalWrite(LED_PINS[ledIndex], LOW);
    delay(tempo);
    // Converte a lista numa String   
    sequenciaNumerica += String(ordemLeds[j]);
  }
}
void buzina (int ton, int tempo){
  int tono = 110 * (ton + 1);
  tone(buzzer,tono,tempo );
}
bool clique(){
  Serial.println("aperte qualquer botao para continuar");
  long startTime = millis();
  for (int i = 0; i < NUM_BUTTONS; i++){
    while (startTime < SERIAL_TIMEOUT){
      if(digitalRead(BUTTONS[i]) == LOW){
      return true;
    }}
    return false;
  }
}
void pontuar(int pontuacao){
  Serial.println("deseja gravar sua pontuação?");
    leserial();
  if (recebido.equalsIgnoreCase("s")){
    leserial();
    //EEPROM.WRITE(seq_pontuacao, pontuacao );
  }
    
}
void ChooseDif(){
  while(true){
    int startTime = millis();
  Serial.println("escolha a difficuldade:\n iniciante\n medio\n hard");
    while (Serial.available() == 0 && millis() - startTime < SERIAL_TIMEOUT)
    {}
    if (Serial.available()) {       
    Dificuldade = Serial.readString();
    break;
    }else{
     Serial.println("por favor escolha uma dificuldade");
}
  }
}
void jogo(){
  nivel = 1;
  int velocidade = 1000;
  if(Dificuldade.equalsIgnoreCase("medio" )){
    velocidade = velocidade * 0.66 ;
  }if(Dificuldade.equalsIgnoreCase("hard") ){
    velocidade = velocidade * 0.33;
  }
  Serial.println(velocidade);
  while(true){
  int piscas = 1 + (2 * nivel);
  geraSequencia(velocidade, piscas);
    if(modo.equals("botaos")){
      lebotaos();
        if (ordemBotaos.equals(sequenciaNumerica)){
    Serial.println("Parabens! proximo nivel.");
    piscaled(1000,3);
    nivel = nivel + 1;
  }else{
    Serial.println("Errooooo!!!"); 
    piscaled(300,5); 
    break;
        }}
    else{
  leserial();
  if (recebido.equals(sequenciaNumerica)){
    Serial.println("Parabens! proximo nivel.");
    piscaled(1000,3);
    nivel = nivel + 1;
  }else{
    Serial.println("Errooooo!!!"); 
    piscaled(300,5); 
    break;
  }}}}

void lebotaos(){
  ordemBotaos = "";
 Serial.println("Aperte os botaos na ordem");
 long startTime = millis();
 while (millis() - startTime < SERIAL_TIMEOUT){
       for (int i = 0; i < NUM_BUTTONS; i++){
         if(digitalRead(BUTTONS[i]) == LOW){
          buzina(i, 200);
          piscarled(i);
      ordemBotaos += String(i + 1);
           Serial.println(ordemBotaos);
         }
       }
 }}

void escolherModo(){
  long startTime = millis();
  Serial.println("deseja jogar com o monitor ou os botaos?");
   while (Serial.available() == 0 && millis() - startTime < SERIAL_TIMEOUT)
  {}
    if (Serial.available()) {
    modo = Serial.readString();
  } else {
    Serial.println("Por favor escolha a opção");
  }
}
    
void piscarled(int led){
  digitalWrite(LED_PINS[led], HIGH);
  delay(400);
  digitalWrite(LED_PINS[led], LOW);
}
