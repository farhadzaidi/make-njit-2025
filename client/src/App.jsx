import LiveStream from "./components/LiveStream";
import Move from "./components/Move";
import PathGrid from "./components/PathGrid";

function App() {
  const esp32_ip = "172.20.10.13";

  return (
    <div>
      {/* <PathGrid /> */}
      <LiveStream esp32_ip={esp32_ip} />
      <Move esp32_ip={esp32_ip} />
    </div>
  )
}

export default App
