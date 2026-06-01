# Plataforma IoT para Gestão da Qualidade do Ar Interior em Ambientes Educacionais

Sistema IoT de monitoramento inteligente de ventilação para salas de aula, desenvolvido como projeto da disciplina de Objetos Inteligentes Conectados da Universidade Presbiteriana Mackenzie. O projeto está alinhado ao ODS 11 da Agenda 2030 da ONU.

## Descrição

O sistema monitora continuamente temperatura, umidade e qualidade do ar em salas de aula, utilizando dois dispositivos ESP32 simulados no Wokwi, broker MQTT HiveMQ, Node-RED na AWS, InfluxDB e Grafana.

## Estrutura do repositório

- `dispositivo1/` — código do ESP32 com sensores DHT22 e MQ-135
- `dispositivo2/` — código do ESP32 com LED RGB e buzzer
- `nodered/` — fluxos exportados do Node-RED em JSON

## Tecnologias utilizadas

- ESP32 (Wokwi)
- MQTT (HiveMQ)
- Node-RED (AWS)
- InfluxDB Cloud
- Grafana Cloud
- OpenWeather API
- Telegram API

## Vídeo de demonstração

Link do YouTube — [LINK](https://youtu.be/lYi108S7h6E)

## Integrantes

- Daniel Bartels de Carli - 10436801
- Luiz Filipe Batista dos Santos - 10438938
- Magno Rogério de Oliveira Junior - 10439896
