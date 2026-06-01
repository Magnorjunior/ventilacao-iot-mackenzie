# Plataforma IoT para Gestão da Qualidade do Ar Interior em Ambientes Educacionais

Sistema IoT de monitoramento inteligente de ventilação para salas de aula, desenvolvido como projeto da disciplina de Objetos Inteligentes Conectados da Universidade Presbiteriana Mackenzie. Alinhado ao ODS 11 da Agenda 2030 da ONU — Cidades e Comunidades Sustentáveis.

## Sumário
1. Visão geral
2. Objetivo do projeto
3. Tecnologias utilizadas
4. Arquitetura do projeto
5. Dispositivos simulados
6. Fluxo Node-RED e regra de negócio
7. Estrutura do repositório
8. Como recriar o projeto
9. Video
10.Integrantes

---

## 1. Visão geral

O sistema monitora continuamente temperatura, umidade e qualidade do ar em salas de aula universitárias. Dois ESP32 simulados no Wokwi coletam dados e publicam via MQTT. O Node-RED processa os dados, consulta a API OpenWeather, envia alertas via Telegram e salva no InfluxDB. Os dados são visualizados em dashboards no Grafana.

## 2. Objetivo do projeto

- Monitorar temperatura, umidade e concentração de gases em salas de aula
- Classificar o ambiente em 3 estados: adequado, atenção e crítico
- Acionar alertas visuais, sonoros e via Telegram automaticamente
- Armazenar histórico e visualizar em dashboards gerenciais

## 3. Tecnologias utilizadas

- ESP32 (Wokwi)
- MQTT (HiveMQ — broker.hivemq.com:1883)
- Node-RED (AWS EC2 t2.medium)
- InfluxDB Cloud (us-east-1-1.aws.cloud2.influxdata.com)
- Grafana Cloud (magnoroj2004.grafana.net)
- OpenWeather API
- Telegram Bot API

## 4. Arquitetura do projeto

 publica via MQTT
 broker.hivemq.com:1883
 Node-RED (AWS)
 Regra de negócio
 penWeather API
 Telegram (alertas)
 InfluxDB (armazenamento)
 publica alerta via MQTT
 Dispositivo 1 (Wokwi)
 Dispositivo 2 (Wokwi)
 LED RGB + Buzzer

 
<img width="639" height="481" alt="topologia" src="https://github.com/user-attachments/assets/d2072a26-e4ce-48de-8f83-ee4e741d919d" />


 ## 5. Dispositivos simulados

### Dispositivo 1 — Coleta de dados
- Sensor DHT22 — temperatura e umidade
- Potenciômetro deslizante — simula sensor MQ-135 (qualidade do ar em PPM)
- Publica a cada 30 segundos nos tópicos:
  - `mackiot/sensor/clima` — temperatura e umidade em JSON
  - `mackiot/sensor/gas` — PPM e valor raw em JSON
  
<img width="638" height="362" alt="dispositivo1" src="https://github.com/user-attachments/assets/81181382-389c-4fa7-8d1a-44dc5f48fc37" />

### Dispositivo 2 — Atuação
- LED vermelho, verde e azul
- Buzzer
- Inscrito no tópico `mackiot/atuador/alerta`
- Reage conforme o nível recebido:
  - `0` = ADEQUADO → LED verde acende
  - `1` = ATENÇÃO → LED amarelo (vermelho + verde) acende
  - `2` = CRÍTICO → LED vermelho acende + buzzer ativado

<img width="914" height="545" alt="dispostivo2" src="https://github.com/user-attachments/assets/44f88765-c23a-45f2-8296-842e6ae22d83" />


## 6. Fluxo Node-RED e regra de negócio

<img width="1417" height="482" alt="fluxonodered" src="https://github.com/user-attachments/assets/210eb264-6e49-4da0-b697-1951a902a8f3" />

O fluxo MacKIoT possui 4 etapas:

**1. Recepção MQTT**
Recebe os dados dos tópicos `mackiot/sensor/clima` e `mackiot/sensor/gas` e armazena nas variáveis globais `temperatura`, `umidade` e `ppm`.

**2. Regra de negócio**
```javascript
if (temp > 31 || umid > 70) {
    nivel = 2; // CRÍTICO
} else if (temp > 26 || umid > 60) {
    nivel = 1; // ATENÇÃO
} else {
    nivel = 0; // ADEQUADO
}
```
O nível calculado é publicado no tópico `mackiot/atuador/alerta`.

**3. APIs externas**
- OpenWeather consultada a cada leitura — retorna temperatura, umidade, vento e condição do céu de São Paulo
- Telegram envia alerta automático quando nível >= 1


**4. Armazenamento**
Três measurements salvos no InfluxDB Cloud no bucket `bucket_dados`:
- `clima` — temperatura e umidade internas
- `gas` — PPM e raw do MQ-135
- `clima_externo` — dados da OpenWeather

## 7. Estrutura do repositório
├── README.md
├── dispositivo1/
│   └── sketch.ino       
├── dispositivo2/
│   └── sketch.ino        
└── nodered/
└── flows.json   

## 8. Como recriar o projeto

### Dispositivos no Wokwi
1. Acesse wokwi.com e crie dois projetos ESP32
2. Importe o `diagram.json` e o `sketch.ino` de cada dispositivo
3. Instale as bibliotecas: `PubSubClient`, `DHTesp`, `ArduinoJson`

### Node-RED na AWS
1. Crie instância EC2 Ubuntu 24 t2.medium
2. Instale Node.js e Node-RED
3. Configure PM2 para inicialização automática
4. Importe o `nodered/flows.json`
5. Configure o token do Telegram e credenciais do InfluxDB

### InfluxDB
1. Crie conta em cloud2.influxdata.com
2. Crie bucket `bucket_dados`
3. Gere token de acesso e configure no Node-RED

### Grafana
1. Crie conta em grafana.com
2. Adicione InfluxDB como data source
3. Crie dashboards com queries Flux nos measurements `clima`, `gas` e `clima_externo`
   
### Dash1
<img width="2560" height="1035" alt="dashbboard1" src="https://github.com/user-attachments/assets/f839706a-5f0a-4202-8d72-0162e2b41410" />

### dash2
<img width="2557" height="1036" alt="dashboprad2" src="https://github.com/user-attachments/assets/318f0fd2-e574-4c6f-ab3b-c43d1a4b98d4" />


### Telegram
1. Crie bot via @BotFather
2. Configure token no nó telegram sender do Node-RED
3. Configure o chatId do destinatário

## 9. Vídeo de demonstração

[Link do YouTube] — [LINk](https://youtu.be/lYi108S7h6E)

## 10. Integrantes

- Daniel Bartels de Carli — 10436801@mackenzista.com.br
- Luiz Filipe Batista dos Santos — 10438938@mackenzista.com.br
- Magno Rogério de Oliveira Junior — 10439896@mackenzista.com.br
