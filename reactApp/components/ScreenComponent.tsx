import React from "react";
import { useTheme } from "../hooks/ThemeContext";
import { ScreenProps } from "../types/ScreenProps";

const ScreenComponent: React.FC<ScreenProps> = ({ title, children }) => {
  const { selectThemeClass } = useTheme();

  return (
    <div
      className={`${selectThemeClass(
        "bg-white",
        "bg-gray-900"
      )} flex flex-col items-center justify-items-center w-screen h-screen`}
      style={{
        position: "relative",
      }}
    >
      <div
        className={`${selectThemeClass(
          "text-black",
          "text-white"
        )} text-3xl font-bold h-1/6 text-center w-full flex flex-row justify-center items-center`}
      >
        <span
          className={`${selectThemeClass(
            "text-black",
            "text-white"
          )} text-3xl font-bold text-center`}
        >
          {title}
        </span>
      </div>
      <div
        className={`flex flex-col w-full h-5/6 ${selectThemeClass(
          "bg-gray-200 text-black",
          "bg-gray-800 text-white"
        )} justify-center items-center`}
      >
        {children}
      </div>
    </div>
  );
};

export default ScreenComponent;
