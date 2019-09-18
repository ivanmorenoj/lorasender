// Decode decodes an array of bytes into an object.
//  - fPort contains the LoRaWAN fPort number
//  - bytes is an array of bytes, e.g. [225, 230, 255, 0]
// The function must return an object, e.g. {"temperature": 22.5}
function Decode(fPort, bytes) {
    var decoded = {};

    // ambiental variables
    decoded.temperatura = bytes[0];
    decoded.humedad    = bytes[1];
    decoded.presion    = (bytes[3] << 8 | bytes[2]) * 100.0;
    
    // gas values
    decoded.co    = (bytes[ 5] << 8 | bytes[ 4]) / 1000.0;
    decoded.o3    = (bytes[ 7] << 8 | bytes[ 6]) / 1000.0;
    decoded.so2   = (bytes[ 9] << 8 | bytes[ 8]) / 1000.0;
    decoded.no2   = (bytes[11] << 8 | bytes[10]) / 1000.0;
    decoded.pm1   = (bytes[13] << 8 | bytes[12]) / 100.0;
    decoded.pm10  = (bytes[15] << 8 | bytes[14]) / 100.0;
    decoded.pm25  = (bytes[17] << 8 | bytes[16]) / 100.0;

    return decoded;
  }