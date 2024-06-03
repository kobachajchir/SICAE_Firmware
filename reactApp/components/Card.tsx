/*
// Card.tsx
import React, { useEffect, useState } from "react";
import { getCurrentState } from "../tools/utils";
import { ConnectionInfo, UserInfo, SystemInfo } from "../types/APITypes";
import { useTheme } from "../hooks/ThemeContext";
//@ts-ignore
import { MaterialCommunityIcons } from "react-web-vector-icons";
import { IconType } from "../types/IconTypes";
import { useNavigate } from "react-router-dom";
import { updateConnectionData } from "../tools/api";

interface CardProps {
  classnames: string;
}

const Card: React.FC<CardProps> = ({ classnames = "" }) => {
  const [currentState, setCurrentState] = useState<"ON" | "OFF">("OFF");
  const { selectThemeClass, isDarkMode } = useTheme();
  const navigate = useNavigate();

  useEffect(() => {
    setCurrentState(getCurrentState(enchufe));
  }, [enchufe]);

  function handleGoToEnchufeDetail() {
    navigate(`/enchufe/${enchufe.id}`);
  }

  const modeColorClass = (mode: string) => {
    switch (mode) {
      case "MANUAL":
        return "text-green-500";
      case "TIMERIZADO":
        return "text-red-500";
      case "PROGRAMADO":
        return "text-blue-500";
      default:
        return "";
    }
  };

  async function handleChangeState() {
    if (enchufe.mode === "MANUAL") {
      //Send off signal, wait for response to change
      console.log(`Trying to change Enchufe ${enchufe.id}`);
      const data = await updateEnchufe(enchufe);
      if (data) {
        setCurrentState(currentState === "OFF" ? "ON" : "OFF");
      }
    }
  }

  return (
    <div
      className={`${selectThemeClass(
        "bg-gray-100 text-black",
        "bg-gray-900 text-white"
      )} 
      rounded-lg p-8 flex flex-col items-center w-1/4 ${classnames}`}
      style={{ borderRadius: "15px" }}
    >
      <div className="flex justify-center items-center mb-2">
        <button className="flex" onClick={handleChangeState}>
          <MaterialCommunityIcons
            name={IconType.Power}
            color={currentState === "ON" ? "#22c55e" : "#ef4444"}
            size={100}
          ></MaterialCommunityIcons>
        </button>
      </div>
      <button
        className="flex flex-col items-center justify-center"
        onClick={handleGoToEnchufeDetail}
      >
        <div className="flex items-center -mb-1">
          <MaterialCommunityIcons
            name={enchufe.iconName}
            color={selectThemeClass("#000", "#fff")}
            size={30}
          ></MaterialCommunityIcons>
        </div>
        <h3 className="text-5xl mb-2 px-3">{enchufe.deviceName}</h3>
        <div
          className={`text-lg font-bold my-1 ${modeColorClass(enchufe.mode)}`}
        >
          {enchufe.mode}
        </div>
      </button>
    </div>
  );
};

export default Card;
*/