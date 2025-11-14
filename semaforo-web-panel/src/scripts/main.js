// main.js

// Define the WebSocket connection
const socket = new WebSocket('ws://<ESP32_IP_ADDRESS>:<PORT>'); // Replace with your ESP32 IP and port

// DOM elements
const nightModeSwitch = document.getElementById('night-mode-switch');
const trafficLightButton = document.getElementById('traffic-light-button');
const ldrReadingDisplay = document.getElementById('ldr-reading');
const trafficLightStateDisplay = document.getElementById('traffic-light-state');
const logsDisplay = document.getElementById('logs');

// Event listener for night mode switch
nightModeSwitch.addEventListener('change', () => {
    document.body.classList.toggle('night-mode', nightModeSwitch.checked);
});

// Event listener for traffic light button
trafficLightButton.addEventListener('click', () => {
    socket.send(JSON.stringify({ action: 'changeTrafficLight' }));
});

// WebSocket event listeners
socket.addEventListener('open', () => {
    console.log('Connected to ESP32');
});

socket.addEventListener('message', (event) => {
    const data = JSON.parse(event.data);
    
    // Update LDR reading
    if (data.ldrReading !== undefined) {
        ldrReadingDisplay.textContent = `LDR Reading: ${data.ldrReading}`;
    }

    // Update traffic light state
    if (data.trafficLightState !== undefined) {
        trafficLightStateDisplay.textContent = `Traffic Light State: ${data.trafficLightState}`;
    }

    // Update logs
    if (data.log) {
        const logEntry = document.createElement('div');
        logEntry.textContent = data.log;
        logsDisplay.appendChild(logEntry);
    }
});

// Handle WebSocket errors
socket.addEventListener('error', (error) => {
    console.error('WebSocket error:', error);
});

// Handle WebSocket closure
socket.addEventListener('close', () => {
    console.log('WebSocket connection closed');
});