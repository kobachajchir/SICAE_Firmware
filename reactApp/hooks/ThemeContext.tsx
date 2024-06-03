import React, { createContext, useContext, useState, ReactNode } from "react";

interface ThemeContextProps {
  isDarkMode: boolean;
  toggleTheme: () => void;
  selectThemeClass: (lightClass: string, darkClass: string) => string;
}

const ThemeContext = createContext<ThemeContextProps | undefined>(undefined);

export const ThemeProvider: React.FC<{ children: ReactNode }> = ({
  children,
}) => {
  const [isDarkMode, setIsDarkMode] = useState(true);

  const toggleTheme = () => {
    setIsDarkMode((prevMode) => !prevMode);
  };

  const selectThemeClass = (lightClass: string, darkClass: string) => {
    return isDarkMode ? darkClass : lightClass;
  };

  return (
    <ThemeContext.Provider
      value={{ isDarkMode, toggleTheme, selectThemeClass }}
    >
      {children}
    </ThemeContext.Provider>
  );
};

export const useTheme = (): ThemeContextProps => {
  const context = useContext(ThemeContext);
  if (!context) {
    throw new Error("useTheme must be used within a ThemeProvider");
  }
  return context;
};
