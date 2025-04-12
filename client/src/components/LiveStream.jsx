function LiveStream() {
  const ESP32_IP = "10.201.97.108";

  return (
    <img src={`http://${ESP32_IP}/stream`} alt="Live Stream" width="300" />
  );

}

export default LiveStream;