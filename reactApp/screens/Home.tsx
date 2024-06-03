import React, { useEffect, useState } from "react";
import { useTheme } from "../hooks/ThemeContext";
import { ConnectionInfo } from "../types/APITypes";
import { useNavigate } from "react-router-dom";
import GoToButton from "../components/GoToButton";
import { IconType } from "../types/IconTypes";
//@ts-ignore
import { MaterialCommunityIcons } from "react-web-vector-icons";

function Home() {
  const [connectionInfo, setconnectionInfo] = useState<ConnectionInfo | null>(null);
  const [loaded, setLoaded] = useState<boolean>(false);
  const { selectThemeClass } = useTheme();
  const navigate = useNavigate();

  useEffect(() => {
    //Call API to get connection Info
  }, []);

  useEffect(() => {
    if (connectionInfo) {
      setLoaded(true);
    }
  }, [connectionInfo]);

  /*if (!loaded) {
    return <div>Loading...</div>;
  }*/

  function handleGoToSettings() {
    navigate("/settings");
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
          Inicio
        </span>
      </div>
      <div
        className={`flex flex-col w-full h-5/6 ${selectThemeClass(
          "bg-gray-200 text-black",
          "bg-gray-800 text-white"
        )} justify-center items-center`}
      >
        <div
          className={`flex w-full flex-col h-3/4 justify-center items-center`}
        >
          <div
            className={`flex w-1/2 rounded-2xl flex-col h-5/6 align-center justify-center items-center ${selectThemeClass(
              "bg-gray-100",
              "bg-gray-900"
            )}`}
            >
              <MaterialCommunityIcons
                name={IconType.Router}
                color={selectThemeClass("#000", "#fff")}
                size={150}
              ></MaterialCommunityIcons>
              <p
              className={`${selectThemeClass(
                "text-black",
                "text-white"
              )} text-2xl font-bold text-center -mt-10 mb-4`}
              >
              {`La placa esta en modo ${connectionInfo?.apMode ? "Wifi" : "AP"}`}
              </p>
              <div className="flex flex-col items-center justify-center my-2">
                <div className="flex flex-row items-center justify-center">
                  <label
                  className={`${selectThemeClass(
                    "text-black",
                    "text-white"
                  )} text-2xl font-bold text-center`}
                  >
                  Wifi SSID: 
                  </label>
                </div>
                <div className="flex flex-row items-center justify-center">
                  <label
                    className={`${selectThemeClass(
                      "text-black",
                      "text-white"
                    )} text-2xl font-bold text-center`}
                    >
                    WIFI Password: 
                  </label>
                </div>
              </div>
              <div className="flex flex-col items-center justify-center my-2">
                <div className="flex flex-row items-center justify-center">
                  <label
                  className={`${selectThemeClass(
                    "text-black",
                    "text-white"
                  )} text-2xl font-bold text-center`}
                  >
                  AP SSID: 
                  </label>
                </div>
                <div className="flex flex-row items-center justify-center">
                  <label
                    className={`${selectThemeClass(
                      "text-black",
                      "text-white"
                    )} text-2xl font-bold text-center`}
                    >
                    AP Password: 
                  </label>
                </div>
              </div>
              <div className="flex flex-row justify-around items-center mb-10 mt-5">
                <button
                  onClick={() => {}}
                  className={`flex flex-row items-center justify-center bg-red-500 h-16 w-16 rounded-full p-2 mx-2`}
                >
                  <MaterialCommunityIcons
                    name={IconType.Restore}
                    color={selectThemeClass("#000", "#fff")}
                    size={40}
                  ></MaterialCommunityIcons>
                </button>
                <button
                  onClick={() => {}}
                  className={`flex flex-row items-center justify-center bg-green-500 h-16 w-16 rounded-full p-2 mx-2`}
                  >
                  <MaterialCommunityIcons
                    name={IconType.Save}
                    color={selectThemeClass("#000", "#fff")}
                    size={40}
                  ></MaterialCommunityIcons>
                </button>
              </div>
            </div>
        </div>
            <div className="h-1/6 flex items-center justify-center align-center">
              <GoToButton
                fnGoTo={handleGoToSettings}
                goToSectionTitle="Configuracion"
                icon={IconType.Next}
                classnames="text-2xl mr-2"
                classnamesContainer={`${selectThemeClass(
                  "bg-gray-100 text-black",
                  "bg-gray-900 text-white"
                )} px-5`}
              ></GoToButton>
            </div>
          </div>
    </div>
  );
}

export default Home;
