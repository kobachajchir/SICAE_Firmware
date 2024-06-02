document.addEventListener("DOMContentLoaded", () => {
  const rootDiv = document.getElementById("root");
  let helloP = document.createElement("p");
  helloP.innerText = "Hola mundo";
  rootDiv.appendChild(helloP);
});
