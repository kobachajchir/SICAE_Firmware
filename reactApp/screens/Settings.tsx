import React, { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";
import { useTheme } from "../hooks/ThemeContext";
import GoToButton from "../components/GoToButton";
import { SystemInfo } from "../types/APITypes";
import { fetchSystemInfo } from "../tools/api";
import ToggleButton from "../components/toggleButton";
import { MdCloudDownload, MdHome } from "react-icons/md";


function Settings() {
  const [systemInfo, setSystemInfo] = useState<
    SystemInfo | null
  >(null);
  const { isDarkMode, selectThemeClass, toggleTheme } = useTheme();
  const navigate = useNavigate();

  async function getData() {
    console.log("get data");
  }

  useEffect(() => {
    if (!systemInfo) {
      getData();
    }
  }, [systemInfo]);

  function handleGoToHome() {
    navigate("/");
  }

  /*if (!systemInfo) {
    return <p>Loading...</p>
  }*/

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
        style={{
          position: "relative",
        }}
      >
        <GoToButton
          goToSectionTitle={"Inicio"}
          fnGoTo={handleGoToHome}
          icon={<MdHome
            fill={selectThemeClass("#000", "#fff")}
            size={35}
          ></MdHome>}
          classnames="ml-2"
          classnamesContainer={`absolute left-0 ml-4 ${selectThemeClass(
            "bg-gray-200 text-black",
            "bg-gray-700 text-white"
          )} flex-row align-center items-center justify-center`}
        ></GoToButton>
        <span
          className={`${selectThemeClass(
            "text-black",
            "text-white"
          )} text-6xl font-bold text-center`}
        >
          Configuracion
        </span>
        <div className="flex flex-row justify-center items-center h-min absolute right-0 mr-10  ">
          <ToggleButton
            onColor="bg-gray-200"
            offColor="bg-gray-900"
            filled={true}
            circleColor={"bg-sky-400"}
            toggle={isDarkMode}
            setToggle={toggleTheme}
            textOn="Oscuro"
            textOff="Claro"
          />
        </div>
      </div>
      <div
        className={`flex flex-col w-full h-5/6 ${selectThemeClass(
          "bg-gray-200 text-black",
          "bg-gray-800 text-white"
        )} items-center`}
      >
        <div
          className={`flex w-full flex-col ${selectThemeClass(
            "bg-gray-200",
            "bg-gray-800"
          )} items-center justify-center`}
        >
          <div className="flex flex-col text-center my-4">
            <h1 className="text-4xl font-bold">
              Informacion de la placa
            </h1>
            {systemInfo && <div className="flex flex-col my-3">
              <p className="text-xl">Chip ID: {systemInfo?.chipId}</p>
              <p className="text-xl">Firmware: {systemInfo?.firmware}</p>
              <p className="text-xl">ESP Time: {systemInfo?.espTime}</p>
            </div>}
          </div>
          <button
            onClick={getData}
            className={`flex flex-row items-center justify-center bg-green-400 h-min rounded-xl px-4 my-2`}
          >
            <MdCloudDownload
              fill={selectThemeClass("#000", "#fff")}
              fontSize={45}
            ></MdCloudDownload>
            <p
              className={`text-2xl ml-2 ${selectThemeClass(
                "text-black",
                "text-white"
              )}`}
            >
              Solicitar informacion
            </p>
          </button>
        </div>
      </div>
    </div>
  );
}

export default Settings;
