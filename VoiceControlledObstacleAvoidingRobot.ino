#include<AFMotor.h>
#include<Servo.h>
#include <NewPing.h>
#define ECHO A4
#define TRIG A5
#define MAX_DIST 300

// DC motori
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Servo motor
Servo servo;

// Ultrasonicni senzor
NewPing sonar(TRIG, ECHO, MAX_DIST);

// Glasovna komanda
String poruka;
String pret_poruka;
String pom_poruka;
String por;
String p;
String q;
char kom;
int pom; // Sluzi za detektovanje nove poruke pri obilasku prepreke
int prepreka;
// Vodi racuna sa promjenama npr sa lijeva na desno sta ce biti sa ovim okret, nisi to rijesila
int okret;
int brzina; //Globalna promjenjiva koja govori koja je trenutna brzina motora
int komanda;
int prekid;
String mode;
int first; // Sluzi za detekciju promjene komande, kako ne bi radio obilazak bez ijedne komande
int change; // Sluzi za detekciju nove komande, da bi se prekinuo obilazak i uradila nova radnja
void setup() {

  // Pokretanje protokola i brzina prenosa podataka
  Serial.begin(9600);

  // Definisanje servo motora
  servo.attach(10);
  servo.write(90);

  // Zaustavljanje svih motora na pocetku
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

  //Postavljanje inicijalne brzine
  Speed(150);
  pom = 0;
  prepreka = 0;
  okret = 0;
  komanda = 0;
  mode = "";
  kom = ' ';
  first = 0;
}
// Postavljanje brzine motora
void Speed(int brz)
{
  // Zastita od vrijednosti preko
  if (brz > 255) brz = 255;
  if (brz < 0) brz = 0;

  brzina = brz;
  motor1.setSpeed(brz);
  motor2.setSpeed(brz);
  motor3.setSpeed(brz);
  motor4.setSpeed(brz);
}

// Kretanje robota unaprijed
void Forward()
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

//Kretanje robota unazad

void Backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

//Kretanje robota lijevo

void Left()
{
  Speed(255);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(350);
  Speed(150);
  okret = 1;
}

//Kretanje robota desno

void Right()
{
  Speed(255);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(350);
  Speed(150);
  okret = 1;
}

// Zaustavljanje

void Stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

// Provjera udaljenosti od prepreke

int Test()
{
  if (udaljenost() < 30)
  {
    return 1;
  }
  return 0;
}

float udaljenost()
{
  float duration = sonar.ping_median(5); // Salje 5 pingova i onda uzima medijanu, radi bolje tacnosti
  float udaljen = (duration / 2) * 0.0343;
  return udaljen;
}

void novaKomanda()
{
  pom_poruka = Serial.readString();
  //  Serial.println("Poruka u gornjoj petlji: " + poruka);
}

// Algoritam za obilazak prepreke

void obilazak()
{
  Stop();
  delay(10);
  servo.write(180);
  delay(1000);
  float left = udaljenost(); //lijevo
  if (left == 0) left = 300;
  servo.write(0);
  delay(1000);
  float right = udaljenost(); //desno
  if (right == 0) right = 300;
  servo.write(90);
  delay(500);
  // Ako se s obje strane nalaze prepreke
  if (Serial.available() > 0)
  {
    pom_poruka = Serial.readString();
    int n = pom_poruka.length();
    poruka = pom_poruka.substring(0, n - 1);
    pom = 1;
    Serial.println("Poruka u obilazak petlji: " + poruka);
  } else
  {
    pom = 0;
    if (left < 50 && right < 50)
    {
      Backward();
      delay(300);
      Stop();
      poruka = "";
      pom = 1; // Da bi se robot zaustavio ako vidi da ne moze ni lijevo ni desno, ceka sledecu komandu!
    } else if (left > right)
    {
      obilazak_s_lijeve();
    } else
    {
      obilazak_s_desne();
    }
  }
}

void prolazak_prepreke(int stepeni)
{
  // Okretanje serva u odredjenu stranu
  prekid = 0;
  Stop();
  delay(500);
  servo.write(90 + stepeni);
  Speed(127);
  delay(500);
  unsigned long start = millis();
  while (udaljenost() < 40)
  {
    if (Serial.available() > 0)
    {
      novaKomanda();
      prekid = -1; // Neka ovo oznacava da je nova komanda stigla
      Serial.println("Usao u citanje u whilu u pp: " + poruka);
      break;
    } else
    {
      Forward();
      if (millis() - start >= 5000)
      {
        prekid = 1;
        break;
      }
    }
  }
  if (prekid == 1)
  {
    Stop();
    poruka = "";
    Speed(150);
    servo.write(90);
  } else if (prekid == -1)
  {
    servo.write(90);
    Speed(150);
    Serial.println("prekid je -1: " + poruka);
  } else
  {
    Forward();
    delay(250);
    servo.write(90);
    Speed(150);
    delay(100);
  }
}

void obilazak_s_lijeve()
{
  if (Serial.available() > 0)
  {
    novaKomanda();
    Serial.println("Usao u obilazak sa lijeve u nk: " + poruka);
  } else
  {
    Backward();
    delay(250);
    Stop();
    delay(1000);
    Speed(150); // Brzina na 200
    Left();
    unsigned long start_time_l = millis();
    prolazak_prepreke(-90);
    unsigned long pass_time_l = millis();
    if (prekid == 1)
    {
      Left();
    } else if (prekid == -1)
    {
      pom = 1;
    } else
    {
      if (Serial.available() > 0)
      {
        novaKomanda();
        prekid = -1;
      }
      Stop();
      delay(1000);
      Right();
      Forward();
      delay(500);
      Stop();
      prolazak_prepreke(-90);
      if (prekid == -1)
      {
        pom = 1;
      } else
      {
        Stop();
        delay(1000);
        if (Serial.available() > 0)
        {
          novaKomanda();
          prekid = -1;
        } else
        {
          Right();
          Speed(127);
          unsigned int start = millis();
          while (millis() - start < pass_time_l - start_time_l - 1350)
          {
            if (Serial.available() > 0)
            {
              novaKomanda();
              prekid = -1;
              break;
            }
            Forward();
          }
          //          delay();
          if (prekid != -1)
          {
            Stop();
            Speed(150);
            delay(1000);
            Left();
            Stop();
            delay(500);
          }
        }
      }
    }
  }
}

void obilazak_s_desne()
{
  if (Serial.available() > 0)
  {
    novaKomanda();
  } else
  {
    Backward();
    delay(250);
    Stop();
    delay(1000);
    Speed(150); // Brzina na 200
    Right();
    unsigned long start_time_r = millis();
    prolazak_prepreke(90);
    unsigned long pass_time_r = millis();
    if (prekid == 1)
    {
      Right();
    } else if (prekid == -1)
    {
      pom = 1;
    } else
    {
      if (Serial.available() > 0)
      {
        novaKomanda();
        prekid = -1;
      }
      Stop();
      delay(1000);
      Left();
      Forward();
      delay(500);
      Stop();
      prolazak_prepreke(90);
      if (prekid == -1)
      {
        pom = 1;
      } else
      {
        Stop();
        delay(1000);
        if (Serial.available() > 0)
        {
          novaKomanda();
          prekid = -1;
        } else
        {
          Left();
          Speed(127);
          unsigned int start = millis();
          while (millis() - start < pass_time_r - start_time_r - 1350)
          {
            if (Serial.available() > 0)
            {
              novaKomanda();
              prekid = -1;
              break;
            }
            Forward();
          }
          if (prekid != -1)
          {
            Stop();
            Speed(150);
            delay(1000);
            Right();
            Stop();
            delay(500);
          }
        }
      }
    }
  }
}

// Funckija za proracun neophodne brzine pri obilasku

// Glavna petlja
void loop()
{
  q = "";
  Serial.println("Pocetak: " + poruka);
  // Slanje podataka preko bluetooth - a
  while (Serial.available())
  {
    pom = 0;
    prekid = 0;
    //    Serial.println("Poruka nova stize");
    prekid = 0;
    okret = 0;
    delay(10);
    char c = Serial.read();
    if (c == '>')
    {
      break;
    }
    q += c;
  }
  //  Serial.println("Poruka prije dodjele q " + poruka);
  Serial.println("Nakon while: " + poruka);

  if (p != q && q != "")
  {
    change = 1;
    p = q;
  }
  Serial.println("Nakon p: " + poruka);

  if (p == "glas" || p == "daljinski")
  {
    mode = p;
    p = "";
    first = 0;
  }

  if (p == "povratak")
  {
    p = "";
    poruka = "";
  }

  if (mode == "glas")
  {
    //    Serial.println("MODE ZA GLAS");
    //      if (Serial.available() > 0)
    //      {
    komanda = 1;
    por = p;
    //        delay(2);
    //        por = Serial.readString();
    //        delay(2);
    if (por != "brže" && por != "sporije" && por != "" && pom == 0)
    {
      poruka = por;
      first = 1;
    } else
    {
      if (por == "brže")
      {
        Speed(brzina + 20);
        p = "";
      } else if (por == "sporije")
      {
        Speed(brzina - 20);
        p = "";
      }
    }
    //      }
    //    Serial.println("Poruka nakon por" + poruka);
    Serial.println("Nakon mode=glas: " + poruka);

    prepreka = Test();
    if (prekid == 1)
    {
      Serial.println("Usao u prekid");
      poruka = "stop";
    } else if (prekid == -1)
    {
      int n = pom_poruka.length();
      poruka = pom_poruka.substring(0, n - 1);
      pom = 1;
    }

    Serial.println("Poruka nakon prekida " + poruka);

    //    if (pret_poruka != poruka)
    //    {
    //      Serial.println("Poruka " + poruka);
    //      Serial.println("Prethodna poruka" + pret_poruka);
    //    }

    if (poruka == "nazad" || poruka == "idi nazad" || poruka == "vrati se")
    {
      Backward();
    }

    if (prepreka == 1 && first == 1 && prekid == 0 && pom == 0)
    {
      obilazak();
    } else
    {
      pret_poruka = poruka;
      //      pom = 0;
      if (poruka == "kreni" || poruka == "naprijed" || poruka == "idi naprijed" || poruka == "napred" || poruka == "idi napred")
      {
        Forward();

      } else if (poruka == "lijevo" || poruka == "idi lijevo" || poruka == "levo" || poruka == "idi levo")
      {
        if (okret == 0)
        {
          Left();
        } else
        {
          Stop();
        }
      } else if (poruka == "desno" || poruka == "idi desno")
      {
        if (okret == 0)
        {
          Right();
        } else
        {
          Stop();
        }
      } else if (poruka == "stop" || poruka == "stani" || poruka == "zaustavi se"  || poruka == "")
      {
        Stop();
        poruka = "";
      }
      komanda = 0;
    }
  } else if (mode == "daljinski")
  {
    unsigned int sto;
    unsigned int s;
    //      if (Serial.available() > 0)
    //      {
    Serial.println("p je: " + p);
    if (p.length() > 0)
    {
      kom = p[0];
    } else
    {
      kom = ' ';
    }
    if (Test() == 1 && kom == 'g')
    {
      Stop();
      kom = ' ';
    } else
    {
      Stop(); //initialize with motors stoped
      //Change pin mode only if new command is different from previous.
      //Serial.println(command);
      switch (kom) {
        case 'g':
          Forward();
          break;
        case 'n':
          Backward();
          break;
        case 'l':
          motor1.run(BACKWARD);
          motor2.run(BACKWARD);
          motor3.run(FORWARD);
          motor4.run(FORWARD);
          break;
        case 'd':
          motor1.run(FORWARD);
          motor2.run(FORWARD);
          motor3.run(BACKWARD);
          motor4.run(BACKWARD);
          break;
        default:
          Stop();
          break;
      }
    }
  }
  //      }
  change = 0;
}
