function LiveStream() {
  const ESP32_IP = "172.20.10.13";

  return (
    <img src={`http://${ESP32_IP}/stream`} alt="Live Stream" width="300" />
  );

}

export default LiveStream;