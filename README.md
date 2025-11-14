# 🟢🔴 Semáforo Inteligente com 2 ESP32 — Projeto IoT

## Sobre o Projeto
Este projeto implementa um sistema de **semáforos inteligentes** utilizando **dois ESP32**, cada um controlando um semáforo independente. Sensores LDR são usados para detectar a presença de veículos e para alternar automaticamente para o **modo noturno** quando a luminosidade do ambiente diminui.

Além disso, o sistema conta com uma **interface online** para controlar parâmetros do semáforo e visualizar em tempo real os dados captados.

---
## Grupo
Tictitas (grupo 5)

## Arquitetura do projeto
├── firmware/
│   ├── semaforo1.ino
│   └── semaforo2.ino
├── interface/
│   ├── index.html
│   └── script.js
├── README.md
│   ├── video-demo.mp4

## Objetivos
- Montar dois semáforos físicos com LEDs e ESP32.  
- Detectar carros simulados usando um **sensor LDR** por variação de luz.  
- Aplicar lógica de controle com **modo normal** e **modo noturno**.  
- Sincronizar comunicação entre os dois ESP32.  
- Criar uma **interface web** simples para ajustes do sistema.  

---

## Componentes Utilizados
- 2× ESP32  
- 1 LDR  
- 2× Resistores de 10kΩ  
- LEDs nas cores: vermelho, amarelo e verde (para cada semáforo)  
- Jumpers, protoboard, fonte USB  
- Navegador + servidor da interface online

---

## Montagem Física

### Conexões do ESP32 (por semáforo)
- LED Vermelho → `GPIO XX`  
- LED Amarelo → `GPIO XX`  
- LED Verde → `GPIO XX`  
- LDR (divisor de tensão) → `GPIO ANALÓGICO XX`

> *Substitua os "XX" pelos pinos usados.*

### Funcionamento do LDR
- Alta iluminação → pouca resistência → leitura analógica baixa  
- Baixa iluminação → alta resistência → leitura analógica alta  

O sistema interpreta:

- **Leitura alta** → “Modo Noturno”  
- **Oscilação rápida** → veículo detectado  

---

## Lógica do Semáforo

### Modo Normal
- Verde → Amarelo → Vermelho  
- Trocas de tempo baseada no fluxo detectado pelo LDR  
- Se um semáforo ficar verde, o outro vai automaticamente para vermelho

### Modo Noturno
- Semáforos piscam **amarelo** lentamente  
- Ativado com baixa luminosidade detectada pelo LDR  
- Pode ser ativado também pela **interface web**

---

## Interface Online
A interface permite:

- Ativar/desativar o **modo noturno**  
- Forçar mudança de estado do semáforo  
- Visualizar em tempo real:
  - Leitura do LDR  
  - Estado atual do semáforo  
  - Atualizações entre os ESP32  

### Link da Interface
`https://seu-link-aqui.com`

## Demonstração do Projeto
 *Adicione aqui o vídeo exigido pela atividade*  
`https://youtu.be/seu-video-aqui`


