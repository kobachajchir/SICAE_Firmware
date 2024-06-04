import React from "react";
import { IconType } from "../types/IconTypes";
import { useTheme } from "../hooks/ThemeContext";
import { MdAccountCircle } from "react-icons/md";

interface GoToButtonProps {
  fnGoTo: () => void;
  goToSectionTitle: string;
  icon: any;
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
      className={`flex ${classnamesContainer} items-center justify-center bg-gray-100 h-min rounded-xl p-2`}
    >
            {icon}
      <p className={classnames}> Ir a {goToSectionTitle}</p>
    </button>
  );
}

export default GoToButton;
