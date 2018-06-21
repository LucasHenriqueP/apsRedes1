# apsRedes1
## RAW socket para criar pacotes ARP, IP e UDP.


Para compilar: gcc redes -o redes -w  
Para executar tenha privilégios __root__  
Antes de usar certifique-se que #define DEFAULT_IF	"enp2s0" esteja o nome da sua placa de rede.  
Como usar: ./redes seuip seuMac ipAlvo  
exemplo: ./redes 10.0.0.2 a0:a1:a2:a3:a4:a5 10.0.0.3  
