function LiveStream() {
  const ESP32_IP = "192.168.50.206";

  return (
    <img src={`http://${ESP32_IP}/stream`} alt="Live Stream" width="300" />
  );

}

export default LiveStream;