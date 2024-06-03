import React, { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";
import { useTheme } from "../hooks/ThemeContext";
//@ts-ignore
import { MaterialCommunityIcons } from "react-web-vector-icons";
import { IconType } from "../types/IconTypes";
import { useUser } from "../hooks/UserContext";

function Login() {
  const [loaded, setLoaded] = useState<boolean>(false);
  const { selectThemeClass } = useTheme();
  const navigate = useNavigate();
  const { user, login, logout, prefersDarkMode, setPrefersDarkMode } =
    useUser();

  useEffect(() => {}, []);

  useEffect(() => {
    setLoaded(true);
    navigate("/home");
  }, []);

  if (!loaded) {
    return <div>Loading...</div>;
  }

  function tryLogin() {
    console.log("login btn");
  }

  return (
    <div
      className={`${selectThemeClass(
        "bg-white",
        "bg-gray-900"
      )} flex flex-col items-center justify-items-center w-full h-full`}
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
          )} text-6xl font-bold text-center`}
        >
          Login
        </span>
      </div>
      <div
        className={`flex flex-col w-full h-5/6 ${selectThemeClass(
          "bg-gray-200 text-black",
          "bg-gray-800 text-white"
        )} justify-center items-center`}
      >
        <div
          className={`flex w-full flex-row h-3/4 ${selectThemeClass(
            "bg-gray-200",
            "bg-gray-800"
          )} justify-center items-center`}
        ></div>
        <div className="h-1/4 flex">
          <button
            onClick={tryLogin}
            className={`flex flex-row items-center justify-center bg-green-400 h-min rounded-xl p-2`}
          >
            <p
              className={`text-lg ${selectThemeClass(
                "text-black",
                "text-white"
              )}`}
            >
              {" "}
              Iniciar sesion
            </p>
            <MaterialCommunityIcons
              name={IconType.Next}
              color={selectThemeClass("#000", "#fff")}
              size={40}
            ></MaterialCommunityIcons>
          </button>
        </div>
      </div>
    </div>
  );
}

export default Login;
