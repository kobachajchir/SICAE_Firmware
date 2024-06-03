import React, { useEffect, useState } from "react";
import { createRoot } from "react-dom/client";
import "./styles.css"; // Importar el CSS aquí
import { ThemeProvider } from "./hooks/ThemeContext";
import { IconType } from "./types/IconTypes";
import { BrowserRouter, Route, Routes } from "react-router-dom";
import Home from "./screens/Home";
import Settings from "./screens/Settings";
import Footer from "./components/Footer";
import { UserProvider, useUser } from "./hooks/UserContext";
import Login from "./screens/Login";
require("react-web-vector-icons/fonts");

/*
            {user && (
              <>
                <Route path="/home" element={<Home />} />
                <Route path="/enchufe/:id" element={<EnchufeDetail />} />
                <Route path="/settings" element={<Settings />} />
              </>
            )}
*/

function Index() {
  const { user } = useUser();
  return (
    <ThemeProvider>
      <BrowserRouter>
        <div className="h-full w-full">
          <Routes>
            <Route path="/" element={<Login />} />

            <Route path="/home" element={<Home />} />
            <Route path="/settings" element={<Settings />} />
          </Routes>
          <Footer></Footer>
        </div>
      </BrowserRouter>
    </ThemeProvider>
  );
}

// Obtén el contenedor en el que deseas renderizar tu aplicación
const container = document.getElementById("root");
const root = createRoot(container!); // Utiliza createRoot en lugar de ReactDOM.render

root.render(
  <UserProvider>
    <Index />
  </UserProvider>
);

export default Index;
