<nav>
  <strong>Table of Contents</strong>
  <ul>
    <li><a href="#introduction">Introduction</a></li>
    <li><a href="#why-this-project-was-created">Why This Project Was Created</a></li>
    <li><a href="#environment-requirements">Environment Requirements</a></li>
    <li><a href="#required-libraries">Required Libraries</a></li>
    <li><a href="#configuration">Configuration</a></li>
    <ul>
      <li><a href="#open-hardware-monitor-ohm">Open Hardware Monitor (OHM)</a></li>
      <li><a href="#project-configuration">Project Configuration</a></li>
    </ul>
    <li><a href="#building-and-running">Building and Running</a></li>
    <li><a href="#connection-troubleshooting">Connection Troubleshooting</a></li>
    <li><a href="#license">License</a></li>
  </ul>
</nav>

# Hardware Measurement Engine

## Introduction

**Hardware Measurement Engine** is a lightweight, object-oriented database engine designed for storing and retrieving hardware measurement time-series (e.g., temperature) in JSON format, with real-time monitoring and timestamp-based indexing capabilities.

## Why This Project Was Created

This project was developed as part of a university assignment. Its goal is to demonstrate a simple non-relational database engine optimized for time-series data from hardware sensors. It includes:

- Real-time monitoring of CPU, GPU, and motherboard metrics  
- Archiving data in JSON with easy export to csv  
- Fast search via indexing by timestamp + component  

## Environment Requirements

The engine was developed and tested in **WSL (Windows Subsystem for Linux)**.  
On other Linux distributions or macOS, you may need to:

- Adjust network/firewall settings for remote access  
- Update file paths in build scripts or configuration files  

## Required Libraries

Install the following dependencies using `apt`:

```bash
sudo apt update
sudo apt install \
  libcurl4-openssl-dev \
  g++ \
  make \
  cmake \
  nlohmann-json3-dev \
  libbson-dev \
  libsqlite3-dev
```
## Configuration

### Open Hardware Monitor (OHM)

1. Download and open [Open Hardware Monitor](https://openhardwaremonitor.org).  
2. In OHM settings, enable **Remote Web Server**.  
3. Note the IP address and port, e.g.: http://192.168.0.100:8080/data.json

### Project Configuration

Open the `include/config/config.h` file and set the OHM server URL:

```c
// include/config/config.h
#define OHM_URL "http://<OHM_IP>:<PORT>/data.json"
```
Replace `<OHM_IP>` and `<PORT>` with the appropriate values.

## Building and Running

In the project’s root directory, execute:
```bash
cd conf
make
make run
```

## Connection Troubleshooting

If WSL cannot connect to the OHM server due to Windows Firewall, open PowerShell as Administrator and run:
```powershell
netsh advfirewall firewall add rule `
  name="OHM WSL Allow" `
  dir=in `
  action=allow `
  protocol=TCP `
  localport=<PORT>
```
Replace `<PORT>` with the port number configured in OHM.

## License
This project is released under the MIT license.
