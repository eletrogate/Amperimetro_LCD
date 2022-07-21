/* =====================================================================================

   Curso de Arduino e AVR WR Kits Channel
   
   Aula 277: Amperímetro com Display LCD 16x2 sem biblioteca
   
   Autor: Eng. Wagner Rambo  
   Data: Janeiro de 2021


===================================================================================== */
 

// =====================================================================================
// --- Mapeamento de Hardware ---
#define  db7  7
#define  db6  6
#define  db5  5
#define  db4  4
#define  db3  11
#define  db2  10
#define  db1  9
#define  db0  8

#define  rs   13
#define  en   12


// =====================================================================================
// --- Constantes do Projeto ---
#define   rshunt   100   //resistor de shunt para mA (0,10 Ohms x 1000)


// =====================================================================================
// --- Protótipo das Funções ---
void char_pos(char row, char col);
void set_col(char address);
void send_char(char character, char row, char col);
void lcd_init();
void lcd_clear();
void ammeter();
float average_volt();
void serial_debug();


// =====================================================================================
// --- Variáveis Globais ---
float content;
unsigned m_amps;

char  mil, cen, dez, uni;
 

// =====================================================================================
// --- Configurações Iniciais ---
void setup()
{
   Serial.begin(9600);
   //configura saídas
   pinMode(db0, OUTPUT);
   pinMode(db1, OUTPUT);
   pinMode(db2, OUTPUT);
   pinMode(db3, OUTPUT);
   pinMode(db4, OUTPUT);
   pinMode(db5, OUTPUT);
   pinMode(db6, OUTPUT);
   pinMode(db7, OUTPUT);
   pinMode(rs,  OUTPUT);
   pinMode(en,  OUTPUT);

   //inicializa saídas
   digitalWrite(db0,  LOW);
   digitalWrite(db1,  LOW);
   digitalWrite(db2,  LOW);
   digitalWrite(db3,  LOW);
   digitalWrite(db4,  LOW);
   digitalWrite(db5,  LOW);
   digitalWrite(db6,  LOW);
   digitalWrite(db7,  LOW);
   digitalWrite(en,   LOW);
   digitalWrite(rs,   LOW);
   delay(2);

   lcd_init();

   send_char('A',1,4);
   send_char('M',1,5);
   send_char('P',1,6);
   send_char('S',1,7);
   send_char(':',1,8);
 
    
} //end setup

 
// =====================================================================================
// --- Loop Infinito ---
void loop()
{
  
  ammeter();

  serial_debug();

  delay(741);
     
} //end loop


// =====================================================================================
// --- Desenvolvimento das Funções ---


void ammeter()
{
  //content = analogRead(A0)*5000.0/1023.0;
  content = (average_volt()/11.0)*(5000.0/1023.0);
  
  content *=1000.0;

  m_amps = (unsigned)(content/rshunt);
  cen = (char)(m_amps/100);
  dez = (char)((m_amps%100)/10);
  uni = (char)(m_amps%10);
  
  send_char(cen+48,2,5);
  send_char(dez+48,2,6);
  send_char(uni+48,2,7);
  send_char('m'   ,2,8);
  send_char('A'   ,2,9); 
  
  
} //end ammeter


void char_pos(char row, char col)
{
   col -= 1;

   switch(row)
   {
      case 1: set_col(col);      break; 
      case 2: set_col(col+0x40); break;
    
   } //end switch
  
} //end char_pos


// =====================================================================================
// --- Função para Envio de Caracteres ---
void set_col(char address)
{
   digitalWrite(rs,   LOW);
   digitalWrite(db7, (address >> 7) | 0x01);
   digitalWrite(db6, (address >> 6) & 0x01);
   digitalWrite(db5, (address >> 5) & 0x01);
   digitalWrite(db4, (address >> 4) & 0x01);
   digitalWrite(db3, (address >> 3) & 0x01);
   digitalWrite(db2, (address >> 2) & 0x01);
   digitalWrite(db1, (address >> 1) & 0x01);
   digitalWrite(db0, (address >> 0) & 0x01);
   digitalWrite(en,   LOW);
   delay(2);
   digitalWrite(en,  HIGH);
   delay(2);
   digitalWrite(en,   LOW);
   delay(2);
   
} //end set_col


void send_char(char character, char row, char col)
{

   char_pos(row, col);
   digitalWrite(db7, (character >> 7) & 0x01);
   digitalWrite(db6, (character >> 6) & 0x01);
   digitalWrite(db5, (character >> 5) & 0x01);
   digitalWrite(db4, (character >> 4) & 0x01);
   digitalWrite(db3, (character >> 3) & 0x01);
   digitalWrite(db2, (character >> 2) & 0x01);
   digitalWrite(db1, (character >> 1) & 0x01);
   digitalWrite(db0, (character >> 0) & 0x01);
 
   digitalWrite(rs,  HIGH);delay(2);
   digitalWrite(en,  HIGH);
   delay(2);
   digitalWrite(en,   LOW);
   
   delay(2);   
  
} //end send_char


// =====================================================================================
// --- Função para Inicializar LCD ---
void lcd_init()
{
   // LIMPA LCD
   digitalWrite(db0, HIGH);
   digitalWrite(db1,  LOW);
   digitalWrite(db2,  LOW);
   digitalWrite(db3,  LOW);
   digitalWrite(db4,  LOW);
   digitalWrite(db5,  LOW);
   digitalWrite(db6,  LOW);
   digitalWrite(db7,  LOW);
   digitalWrite(en,   LOW);
   delay(2);
   digitalWrite(en,  HIGH);
   delay(2);
   digitalWrite(en,   LOW);
   delay(2);

   // MODO DE 8 BITS
   digitalWrite(db0,  LOW);
   digitalWrite(db1,  LOW);
   digitalWrite(db2,  LOW);
   digitalWrite(db3, HIGH);
   digitalWrite(db4, HIGH);
   digitalWrite(db5, HIGH);
   digitalWrite(db6,  LOW);
   digitalWrite(db7,  LOW);
   digitalWrite(en,   LOW);
   delay(2);
   digitalWrite(en,  HIGH);
   delay(2);
   digitalWrite(en,   LOW);
   delay(2);

   // LIGA LCD, LIGA CURSOR, DESLIGA BLINK
   digitalWrite(db0,  LOW);
   digitalWrite(db1, HIGH);
   digitalWrite(db2, HIGH);
   digitalWrite(db3, HIGH);
   digitalWrite(db4,  LOW);
   digitalWrite(db5,  LOW);
   digitalWrite(db6,  LOW);
   digitalWrite(db7,  LOW);
   digitalWrite(en,   LOW);
   delay(2);
   digitalWrite(en,  HIGH);
   delay(2);
   digitalWrite(en,   LOW);
   delay(2);

   // HABILITA INCREMENTO, DESLIGA SCROLL
   digitalWrite(db0,  LOW);
   digitalWrite(db1, HIGH);
   digitalWrite(db2, HIGH);
   digitalWrite(db3,  LOW);
   digitalWrite(db4,  LOW);
   digitalWrite(db5,  LOW);
   digitalWrite(db6,  LOW);
   digitalWrite(db7,  LOW);
   digitalWrite(en,   LOW);
   delay(2);
   digitalWrite(en,  HIGH);
   delay(2);
   digitalWrite(en,   LOW);
   delay(2);
  
} //end lcd_init



void lcd_clear()
{

   // LIMPA LCD
   digitalWrite(rs,   LOW);
   digitalWrite(db0, HIGH);
   digitalWrite(db1,  LOW);
   digitalWrite(db2,  LOW);
   digitalWrite(db3,  LOW);
   digitalWrite(db4,  LOW);
   digitalWrite(db5,  LOW);
   digitalWrite(db6,  LOW);
   digitalWrite(db7,  LOW);
   digitalWrite(en,   LOW);
   delay(2);
   digitalWrite(en,  HIGH);
   delay(2);
   digitalWrite(en,   LOW);
   delay(2);

} //end lcd_clear




float average_volt()                            //Função que calcula a média de 100 leituras de tensão
{                                              //Garante maior precisão na medida
     unsigned char i;                          //Variável de iterações
     float volt_store = 0;                      //Variável local para armazenar o valor da tensão

     for(i=0; i<100; i++)                      //Somatório de 100 leituras
     {
              volt_store += analogRead(A0);       //faz o somatório das 100 iterações
     }

     return(volt_store/100.0);                   //retorna a média das iterações

} //end average_temp


void serial_debug()
{
  Serial.print("analogRead(0)= ");
  Serial.print(analogRead(A0));
  Serial.print("  |  ");
  Serial.print("average_volt= ");
  Serial.print(average_volt());
  Serial.print("  |  ");
  Serial.print("content= ");
  Serial.print(content);
  Serial.print("  |  ");
  Serial.print("mAmps= ");
  Serial.print(m_amps);
  Serial.print(" mA");
  Serial.println("  |  ");
  
  
  
} //end serial_debug

// =====================================================================================
// --- Final do Programa ---