import React from "react";
import { useTheme } from "../hooks/ThemeContext";

function Footer() {
  const { selectThemeClass } = useTheme();
  return (
    <footer
      className={`${selectThemeClass(
        "bg-white",
        "bg-gray-900"
      )} flex flex-row items-center justify-center`}
    >
      <div
        className={`${selectThemeClass(
          "text-black opacity-80",
          "text-white opacity-80"
        )} flex flex-row`}
      >
        <p className="text-md">SICAE Firmware Web App - Koba Chajchir - 2024</p>
      </div>
    </footer>
  );
}

export default Footer;
