sudo apt install libcurl4-openssl-dev
sudo apt install g++ make cmake
sudo apt install nlohmann-json3-dev
sudo apt install libbson-dev

netsh advfirewall firewall add rule name="OHM WSL Allow" dir=in action=allow protocol=TCP localport=8080
