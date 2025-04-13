import React, { useEffect } from 'react';

const Move = ({ esp32_ip }) => {
  const sendMoveSignal = (action) => {
    fetch(`http://${esp32_ip}/move?action=${action}`)
      .then((response) => response.text())
      .then((data) => {
        console.log("Success message from ESP:", data);
      })
      .catch((error) => {
        console.error("Error while sending move command:", error);
      });
  }

  useEffect(() => {
    const handleKeyDown = (e) => {
      // Check if the 'w' key is pressed
      const key = e.key.toLowerCase();
      if (key === 'w' || key === "arrowup") {
        console.log("sending forward");
        sendMoveSignal("forward");
      } else if (key === "s" || key === "arrowdown") {
        console.log("sending reverse");
        sendMoveSignal("reverse");
      } else if (key === "d" || key === "arrowright") {
        console.log("sending clockwise");
        sendMoveSignal("clockwise");
      } else if (key === "a" || key === "arrowleft") {
        console.log("sending counter_clockwise");
        sendMoveSignal("counter_clockwise");
      } else if (key === "m") {
        console.log("sending activate_servo");
        sendMoveSignal("activate_servo");
      } else if (key === "n") {
        console.log("sending deactivate_servo");
        sendMoveSignal("deactivate_servo");
      } else if (key === " ") { // spacebar
        console.log("sending stop");
        sendMoveSignal("stop")
      }
    };

    // Add keydown event listener on mount
    window.addEventListener('keydown', handleKeyDown);

    // Cleanup event listener on unmount
    return () => {
      window.removeEventListener('keydown', handleKeyDown);
    };
  }, []);

  return (
    <div>
      <h1>Motor Control</h1>
      <p>Press <strong>W</strong> to move forward.</p>
    </div>
  );
};

export default Move;
