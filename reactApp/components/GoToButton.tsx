import React from "react";
//@ts-ignore
import { MaterialCommunityIcons } from "react-web-vector-icons";
import { IconType } from "../types/IconTypes";
import { useTheme } from "../hooks/ThemeContext";

interface GoToButtonProps {
  fnGoTo: () => void;
  goToSectionTitle: string;
  icon: string;
  classnames?: string;
  classnamesContainer?: string;
}

function GoToButton({
  fnGoTo,
  icon,
  goToSectionTitle,
  classnames = "",
  classnamesContainer = "",
}: GoToButtonProps) {
  const { selectThemeClass } = useTheme();

  return (
    <button
      onClick={fnGoTo}
      className={`flex ${
        icon === IconType.Back ? "flex-row" : "flex-row-reverse"
      } ${classnamesContainer} items-center justify-center bg-gray-100 h-min rounded-xl p-2`}
    >
      <MaterialCommunityIcons
        name={icon}
        color={selectThemeClass("#000", "#fff")}
        size={40}
      ></MaterialCommunityIcons>
      <p className={classnames}> Ir a {goToSectionTitle}</p>
    </button>
  );
}

export default GoToButton;
