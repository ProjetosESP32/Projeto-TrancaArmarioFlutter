#include <Arduino.h>
#line 1 "C:\\Users\\Allan Franco\\Downloads\\FirebaseTranca\\FirebaseTranca.ino"

#include <FS.h>
#include <SPIFFS.h>
#include <IOXhop_FirebaseESP32.h>
#include <string.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Keypad.h>
#include <Wire.h>
#include <DS3231.h>
#include "DFRobotDFPlayerMini.h"

// WIFI
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
//////////////////

// Dados de Conexao WIFI
#define CON_WIFI_SSID   "Vivabela_Router"
#define CON_WIFI_PASS   "07092002"
uint8_t IP_dns[4] = {192, 168, 100, 1}; // IP do DNS[do gateway]
uint8_t IP_wifi[4] = {192, 168, 1, 240}; // IP do dispositivo
uint8_t IP_gw[4] = {192,168,100,127}; // IP do gateway
uint8_t IP_mask[4] = {255, 255, 255, 0}; // IP Mascara
uint8_t mac[6] = {'\0'};
//////////////////

#define switchmagnetico 32
#define rele 12


TaskHandle_t Task1;
TaskHandle_t Task2;

DS3231 rtc;
RTCDateTime dataehora;
HardwareSerial PlayerMP3(0);
DFRobotDFPlayerMini myDFPlayer;

int i, ValidadeSenha[100], WC = 10, SPIcont = 0;
int cont = 0, x = 0, y = 1, Z, c, aux, M, auxEmail = 0, aX, aY , TempoCriado, Botao, porta;
char apertatecla, confirmacao, ValorDigitado[6];
long tempopassado, tempoatual, temporelativo = 8000, temposwitch = 10000, temposwitchpassado, tempoporta, delayporta = 8000, tempoBloqueio, check_wifi;

const byte linhas = 4;
const byte colunas = 4;

String Senha, QntdSenha[100], Teste[50], Nomes[100];
String Ano[100], Mes[100], Dia[100], Hora[100], Minutos[100], Segundos[100], PlanoEmail, SenhaEmail[100], ValidadeEmail[100], NomesSenha[100], Listavalidade;
String Armario, ArmarioConc, ArmarioLast;
String SenhaGerada, SenhaGeradaConc;
String Proprietario, ProprietarioConc;
String Validade, ValidadeConc;
String Bloqueio, incomingByte, SenhaAnalize;

char matrizteclado[linhas][colunas] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};


static uint8_t taskCoreZero = 0;
static uint8_t taskCoreOne  = 1;

byte pinoslinhas[linhas] = {4, 18, 19, 33};
byte pinoscolunas[colunas] = {25, 26, 27, 13};

Adafruit_SH1106 oled(21, 22);
Keypad teclado = Keypad( makeKeymap(matrizteclado), pinoslinhas, pinoscolunas, linhas, colunas );




const int WIFI_CONNECTED_BIT = BIT0;

// Grupo de Eventos
static EventGroupHandle_t wifi_event_group; // WIFI

static const char *TAG = "WIFI: ";
int idS = 1;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
  switch (event->event_id) {
    case SYSTEM_EVENT_STA_START:
      esp_wifi_connect();
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      ESP_LOGI(TAG, "got ip:%s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
      ESP_ERROR_CHECK(esp_read_mac(mac, ESP_MAC_WIFI_STA));
      //tentativas = 0;
      xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      //esp_wifi_stop();
      //esp_wifi_start();
      esp_wifi_connect();
      xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
      break;
    default:
      break;
  }
  return ESP_OK;
}

void iniciar_wifi() {
  tcpip_adapter_init();
  tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);
  tcpip_adapter_ip_info_t ipInfo;
  tcpip_adapter_dns_info_t dns_info;
  IP_ADDR4(&dns_info.ip, IP_dns[0], IP_dns[1], IP_dns[2], IP_dns[3]); // ALTERAR SE FOR NECESSÁRIO
  IP4_ADDR(&ipInfo.ip, IP_wifi[0], IP_wifi[1], IP_wifi[2], IP_wifi[3]); // ALTERAR SE FOR NECESSÁRIO
  IP4_ADDR(&ipInfo.gw, IP_gw[0], IP_gw[1], IP_gw[2], IP_gw[3]); // ALTERAR SE FOR NECESSÁRIO
  IP4_ADDR(&ipInfo.netmask, IP_mask[0], IP_mask[1], IP_mask[2], IP_mask[3]); // ALTERAR SE FOR NECESSÁRIO
  tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
  tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, TCPIP_ADAPTER_DNS_MAIN, &dns_info);

  ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL) );

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  wifi_config_t wifi_config = {
    .sta = {
      {.ssid = CON_WIFI_SSID},
      {.password = CON_WIFI_PASS}
    },
  };
  //ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
  //ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "wifi_init_sta finished.");
  ESP_LOGI(TAG, "connect to ap SSID:%s password:%s", CON_WIFI_SSID, CON_WIFI_PASS);
}


void setup() {
  
  // -----------------------------------------------------------------------
  // Inicialização da memória não volátil para armazenamento de dados (Non-volatile storage (NVS)).
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  iniciar_wifi(); // Inicia o WIFI
  // -----------------------------------------------------------------------
  // Criando grupos de eventos
  
  wifi_event_group = xEventGroupCreate();

  Serial.begin(9600);
  oled.begin(SH1106_SWITCHCAPVCC, 0x3C);
  SPIFFS.begin();
  rtc.begin();
  Firebase.begin("https://armario-49f10.firebaseio.com/", "VcoRkv7IzUXdpYutUnIY8KD7FLTJKERFyPzIgt62");
  //  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  oled.clearDisplay();


  pinMode(rele, OUTPUT);
  pinMode(switchmagnetico, INPUT);

  Serial.print("connecting");
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(10, 20);
  oled.print("Conectando ao Wifi");

  //  while (WiFi.status() != WL_CONNECTED) {
  //    Serial.print(".");
  //    oled.setTextSize(1);
  //    oled.setTextColor(WHITE);
  //    oled.setCursor(WC, 42);
  //    oled.print(".");
  //    oled.display();
  //    WC += 10;
  //    delay(2);
  //    if (WC >= 118) {
  //      WC = 10;
  //      oled.clearDisplay();
  //      oled.setTextSize(1);
  //      oled.setTextColor(WHITE);
  //      oled.setCursor(10, 20);
  //      oled.print("Conectando ao Wifi");
  //    }
  //  }
  //  oled.clearDisplay();
  //
  //  if (WiFi.status() == WL_CONNECTED) {
  //    Serial.println();
  //    Serial.print("connected: ");
  //    Serial.println(WiFi.localIP());
  //    delay(500);
  //    oled.clearDisplay();
  //    oled.setTextSize(1);
  //    oled.setTextColor(WHITE);
  //    oled.setCursor(34, 20);
  //    oled.print("Conectado");
  //    oled.setCursor(25, 50);
  //    oled.print(WiFi.localIP());
  //    oled.display();
  //    delay(1500);
  //  }




  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////

  File SenhaFFS = SPIFFS.open("/Senhas.txt", "r");
  if (!SenhaFFS) {
    Serial.println("ERRO SPIFFS SENHAS");
  }

  else {
    Serial.println();
    Serial.println("Senhas ===============");
    Serial.println();
    for (SPIcont = 0; SPIcont <= 5; SPIcont ++) {
      Serial.println();
      Serial.print(SPIcont);
      Serial.print(" - ");
      Serial.print(SenhaFFS.readStringUntil('\n'));
    }
  }
  Serial.println();
  Serial.println("=======================");
  Serial.println();
  SenhaFFS.close();


  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////


  File ValidadeFFS = SPIFFS.open("/Validades.txt", "r");
  if (!ValidadeFFS) {
    Serial.println("ERRO SPIFFS VALIDADE");
  }

  else {
    Serial.println();
    Serial.println("Validades ===============");
    Serial.println();
    for (SPIcont = 0; SPIcont <= 5; SPIcont ++) {
      String Val = ValidadeFFS.readStringUntil('\n');
      ValidadeSenha[SPIcont] = Val.toInt();
      Serial.println();
      Serial.print(SPIcont);
      Serial.print(" - ");
      Serial.print( ValidadeSenha[SPIcont]);
    }
  }
  Serial.println();
  Serial.println("=======================");
  Serial.println();
  ValidadeFFS.close();

  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////


  File ProprietarioFFS = SPIFFS.open("/Proprietario.txt", "r");
  if (!ProprietarioFFS) {
    Serial.println("ERRO SPIFFS Proprietario");
  }

  else {
    Serial.println();
    Serial.println("Proprietario =================");
    Serial.println();
    for (SPIcont = 0; SPIcont <= 5; SPIcont ++) {
      Serial.println();
      Serial.print(SPIcont);
      Serial.print(" - ");
      Serial.print(ProprietarioFFS.readStringUntil('\n'));
    }
  }
  Serial.println();
  Serial.println("=======================");
  Serial.println();

  ProprietarioFFS.close();


  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////


  PlayerMP3.begin(9600, SERIAL_8N1, 3, 1);  // speed, type, RX, TX
  if (!myDFPlayer.begin(PlayerMP3))
  {
    Serial.println(F("Nao inicializado:"));
    Serial.println(F("1.Cheque as conexoes do DFPlayer Mini"));
    Serial.println(F("2.Insira um cartao SD"));
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(15, 10);
    oled.print(" X Player MP3 X");
    oled.display();
    delay(3000);
  } else {
    //Definicoes iniciais MP3
    myDFPlayer.setTimeOut(500); //Timeout serial 500ms
    myDFPlayer.volume(30); //Volume
    myDFPlayer.EQ(0); //Equalizacao normal
    myDFPlayer.stop();
  }

  rtc.setDateTime(__DATE__, __TIME__);

  myDFPlayer.play(1);

  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////

  oled.clearDisplay();
  oled.display();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(10, 15);
  oled.print("Iniciando o sistema");
  oled.display();
  delay(1000);
  oled.clearDisplay();

  /////////////////////////////////////////////////////////////////////////////////
  delay(500);

  /////////////////////////////////////////////////////////////////////////////////
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);

}
/////////////////////////////////////////////////////////////////////////////////

void loop() {

  dataehora = rtc.getDateTime();
  apertatecla = teclado.getKey();


  //TELA HIBERNAGEM, CASO NÃO OCORRA NEM UMA AÇÃO////////////////////////////////////////////////////////////////////////////
  if (millis() - tempopassado > 8000L) {
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(WHITE);

    oled.setCursor(25, 20);
    oled.print("LAB-117");
    oled.setCursor(60, 45);
    oled.print(":");

    oled.setCursor(40, 45);
    oled.print(dataehora.minute);
    oled.setCursor(70, 45);
    oled.print(dataehora.second);

    oled.display();
    cont = 0;
    Serial.println(dataehora.second);
  }

  if (digitalRead(switchmagnetico) == LOW) {

    if (millis() - tempoporta > delayporta) {
      if (millis() - temposwitchpassado > temposwitch) {

        temposwitchpassado = millis();
        delay(1200);
        myDFPlayer.play(4);
      }
    }
  } else {
    tempoporta = millis();
  }

 //VERIFICAÇÃO DIÁRIA DO RTC PARA O DECREMENTO DAS VALIDADES as 23:30:10/////////////////////////////////////////
 //DECREMENTAÇÃO DAS VALIDADES///////////////////////////////////////////////////////////////////////////////////

  if (dataehora.second != 10) {
    c = 1;
  }

  if (dataehora.second == 10 && c == 1) {
    aux = 0;
    do {
//
//      if (ValidadeSenha[aux] >= 1) {
//        Serial.print(ValidadeSenha[aux]);
//        ValidadeSenha[aux] --;
//      }

      aux++;
      delay(1);
    } while (aux <= 100);
    c = 0;
  }

  //LÊ CADA TECLA QUE PRESSIONOU E ALOCA NO SEU PROXIMO ESPAÇO/////////////////////////////////////////////////////////////////
  if (apertatecla) {

    tempopassado =  millis();

    switch (cont) {
      case 0:
        ValorDigitado[0] = apertatecla;
        if (ValorDigitado[0] == '*') {
          oled.clearDisplay();
          oled.display();
          cont = -1;
          break;
        }
        oled.clearDisplay();
        oled.setTextSize(2);
        oled.setTextColor(WHITE);
        oled.setCursor(20, 15);
        oled.print(ValorDigitado[0]);
        oled.display();
        cont++;
        delay(100);
        break;
      case 1:
        ValorDigitado[1] = apertatecla;
        if (ValorDigitado[1] == '*') {
          oled.clearDisplay();
          oled.display();
          cont = -1;
          break;
        }
        oled.setTextSize(2);
        oled.setTextColor(WHITE);
        oled.setCursor(35, 15);
        oled.print(ValorDigitado[1]);
        oled.display();
        cont++;
        delay(100);
        break;
      case 2:
        ValorDigitado[2] = apertatecla;
        if (ValorDigitado[2] == '*') {
          oled.clearDisplay();
          oled.display();
          cont = -1;
          break;
        }
        oled.setTextSize(2);
        oled.setTextColor(WHITE);
        oled.setCursor(50, 15);
        oled.print( ValorDigitado[2]);
        oled.display();
        cont++;
        delay(100);
        break;
      case 3:
        ValorDigitado[3] = apertatecla;
        if (ValorDigitado[3] == '*') {
          oled.clearDisplay();
          oled.display();
          cont = -1;
          break;
        }
        oled.setTextSize(2);
        oled.setTextColor(WHITE);
        oled.setCursor(65, 15);
        oled.print( ValorDigitado[3]);
        oled.display();
        cont++;
        delay(100);
        break;
      case 4:
        ValorDigitado[4] = apertatecla;
        if (ValorDigitado[4] == '*') {
          oled.clearDisplay();
          oled.display();
          cont = -1;
          break;
        }
        oled.setTextSize(2);
        oled.setTextColor(WHITE);
        oled.setCursor(80, 15);
        oled.print( ValorDigitado[4]);
        oled.display();
        cont++;
        delay(100);
        break;
      case 5:
        ValorDigitado[5] = apertatecla;
        if (ValorDigitado[5] == '*') {
          oled.clearDisplay();
          oled.display();
          cont = -1;
          break;
        }
        oled.setTextSize(2);
        oled.setTextColor(WHITE);
        oled.setCursor(95, 15);
        oled.print( ValorDigitado[5]);
        oled.display();
        delay(100);
        cont++;
        break;
      case 6:
        if (apertatecla == '#') {
          cont++;
          tempopassado =  millis();
        }
        break;
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  //VERIFICAÇÃO DOS CARACTERES E VALIDADE/////////////////////////////////////////////////////////////////
  if (cont == 7) {
    File SenhaFFS = SPIFFS.open("/Senhas.txt", "r");

    while (SenhaFFS.available()) {

      SenhaAnalize = SenhaFFS.readStringUntil('\n');

      if (!SenhaFFS.available()) {
        Z = 100;
      }

      if ( ValorDigitado[0] == SenhaAnalize[0] && ValorDigitado[1] == SenhaAnalize[1] && ValorDigitado[2] == SenhaAnalize[2] && ValorDigitado[3] == SenhaAnalize[3] && ValorDigitado[4] == SenhaAnalize[4] && ValorDigitado[5] == SenhaAnalize[5]) {
        if (ValidadeSenha[Z] > 0) {

          //SENHA CERTA///////////////////////////////////////////////////////////////////////////////

          tempopassado =  millis();

          oled.clearDisplay();
          oled.setTextSize(1);
          oled.setTextColor(WHITE);
          oled.setCursor(41, 15);
          oled.print("BEM VINDO");
          oled.display();

          delay(100);
          digitalWrite(rele, HIGH);
          myDFPlayer.play(2);
          delay(2000);
          digitalWrite(rele, LOW);
          delay(1000);
          oled.clearDisplay();
          oled.display();

          cont = 0;
        }
      }
    }

    SenhaFFS.close();

    if (Z == 100) {

      //SENHA INEXISTENTE//////////////////////////////////////////////////////////////////////////////
      tempopassado =  millis();
      myDFPlayer.play(3);
      delay(1);
      oled.clearDisplay();
      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(31, 15);
      oled.print("ACESSO NEGADO");
      oled.display();
      delay(1500);
      oled.clearDisplay();
      oled.display();
      cont = 0;
    }
  }

  delay(1);

  //  if ((WiFi.status() != WL_CONNECTED) && (millis() > check_wifi)) {
  //    Serial.println("Reconnecting to WiFi...");
  //    WiFi.disconnect();
  //    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //    check_wifi = millis() + 15000;
  //    delay(10);
  //
  //  }

}



void Task1code( void * pvParameters ) {
  while (true) {

    Bloqueio = Firebase.getString("/ARMARIO/Bloqueio");

    if (Bloqueio == "false") {
      Armario = Firebase.getString("/ARMARIO/Arm%C3%A1rio");
      Proprietario = Firebase.getString("/ARMARIO/Proprietario");
      SenhaGerada = Firebase.getString("/ARMARIO/SenhaGerada");
      Validade = Firebase.getString("/ARMARIO/Validade");

      for (i = 2; i < Armario.length() - 2; i++) {
        ArmarioConc += Armario[i];
      }

      for (i = 2; i < Proprietario.length() - 2; i++) {
        ProprietarioConc += Proprietario[i];
      }

      for (i = 2; i < Validade.length() - 2; i++) {
        ValidadeConc += Validade[i];
      }

      Firebase.set("/ARMARIO/Bloqueio", "true");
      Firebase.set("/ARMARIO/Envio", "true");

      ////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////

      File SenhaFFS = SPIFFS.open("/Senhas.txt", "a+");

      while (!SenhaFFS.available()) {
        SenhaFFS.println(SenhaGerada);

      }
      SenhaFFS.close();

      ////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////

      File ValidadeFFS = SPIFFS.open("/Validades.txt", "a+");

      while (!ValidadeFFS.available()) {
        ValidadeFFS.println(ValidadeConc.toInt());
      }

      ValidadeFFS.close();


      ////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////
      File ProprietarioFFS = SPIFFS.open("/Proprietario.txt", "a+");

      while (!ProprietarioFFS.available()) {
        ProprietarioFFS.println(ProprietarioConc);
      }
      ProprietarioFFS.close();


      ////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////

      ////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////


      QntdSenha[x] = SenhaGerada;
      ValidadeSenha[x] = ValidadeConc.toInt();
      delay(1000);
      Serial.println(ArmarioConc);
      Serial.println(ProprietarioConc);
      Serial.println(SenhaGerada);
      Serial.println(ValidadeConc);
      ValidadeConc = ProprietarioConc = ArmarioConc = "";
      x++;
      
    }
    delay(10);
  }
}
