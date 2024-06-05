import React, { useEffect, useState } from "react";
import { useTheme } from "../hooks/ThemeContext";
import { ConnectionInfo } from "../types/APITypes";
import { useNavigate } from "react-router-dom";
import GoToButton from "../components/GoToButton";
import { MdRestore, MdSave, MdSettings, MdWifi, MdWifiTethering } from "react-icons/md";
import { fetchConnectionInfo, updateConnectionData } from "../tools/api";

function Home() {
  const [connectionInfo, setConnectionInfo] = useState<ConnectionInfo | null>(null);
  const [loaded, setLoaded] = useState<boolean>(false);
  const { selectThemeClass } = useTheme();
  const navigate = useNavigate();

  async function getConnectionInfo() {
    try {
      const data = await fetchConnectionInfo();
      setConnectionInfo(data);
    } catch (error) {
      console.error("Error fetching connection info:", error);
    }
  }

  async function saveConnectionInfo() {
    console.log("saveConnectionInfo");
    if (connectionInfo) {
      try {
        console.log("Datos a enviar:", connectionInfo);
        const success = await updateConnectionData(connectionInfo);
        if (success) {
          console.log("Connection info updated successfully");
        } else {
          console.error("Error updating connection info");
        }
      } catch (error) {
        console.error("Error updating connection info:", error);
      }
    }
  }
  

  function handleRestore() {
    console.log("handleRestore")
    getConnectionInfo();
  }

  useEffect(() => {
  }, []);
/*
  useEffect(() => {
    if (connectionInfo) {
      setLoaded(true);
    }
  }, [connectionInfo]);
*/
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
            {connectionInfo?.apMode === true ?
              <MdWifiTethering
                fill={selectThemeClass("#000", "#fff")}
                size={120}
                style={{margin: 30}}
                ></MdWifiTethering> : <MdWifi
                fill={selectThemeClass("#000", "#fff")}
                size={120}
                style={{margin: 30}}
            ></MdWifi>}
              <p
              className={`${selectThemeClass(
                "text-black",
                "text-white"
              )} text-2xl font-bold text-center -mt-10 mb-4`}
              >
              {`La placa esta en modo ${connectionInfo?.apMode === true ? "AP" : "WIFI"}`}
              </p>
              <div className="flex flex-col items-center justify-center my-2">
                <div className="flex flex-row items-center justify-center">
                  <label
                  className={`${selectThemeClass(
                    "text-black",
                    "text-white"
                  )} text-2xl font-bold text-center`}
                  >
                  WIFI SSID: 
                </label>
                <input
                  className={`${selectThemeClass(
                    "bg-gray-300 text-black",
                    "bg-gray-700 text-white"
                  )} rounded-xl ml-2 text-center`}
                  type="text"
                  name="wifiSsid"
                  id="wifiSsid"
                  value={connectionInfo?.wifiSsid || ""}
                  onChange={(e) => {
                    if (connectionInfo) {
                      setConnectionInfo({ ...connectionInfo, wifiSsid: e.target.value })
                    }
                    } 
                  }
                />
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
                <input
                  className={`${selectThemeClass(
                    "bg-gray-300 text-black",
                    "bg-gray-700 text-white"
                  )} rounded-xl ml-2 text-center`}
                  type="text"
                  name="wifiPassword"
                  id="wifiPassword"
                  value={connectionInfo?.wifiPassword || ""}
                  onChange={(e) => {
                    if (connectionInfo) {
                      setConnectionInfo({ ...connectionInfo, wifiPassword: e.target.value })
                    }
                    } 
                  }
                />
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
                <input
                  className={`${selectThemeClass(
                    "bg-gray-300 text-black",
                    "bg-gray-700 text-white"
                  )} rounded-xl ml-2 text-center`}
                  type="text"
                  name="apSsid"
                  id="apSsid"
                  value={connectionInfo?.apSsid || ""}
                  onChange={(e) => {
                    if (connectionInfo) {
                      setConnectionInfo({ ...connectionInfo, apSsid: e.target.value })
                    }
                    } 
                  }
                />
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
                <input
                  className={`${selectThemeClass(
                    "bg-gray-300 text-black",
                    "bg-gray-700 text-white"
                  )} rounded-xl ml-2 text-center`}
                  type="text"
                  name="apPassword"
                  id="apPassword"
                  value={connectionInfo?.apPassword || ""}
                  onChange={(e) => {
                    if (connectionInfo) {
                      setConnectionInfo({ ...connectionInfo, apPassword: e.target.value })
                    }
                    } 
                  }
                />
                </div>
              </div>
              <div className="flex flex-row justify-around items-center mb-10 mt-5">
                <button
                  onClick={handleRestore}
                  className={`flex flex-row items-center justify-center bg-red-500 h-16 w-16 rounded-full p-2 mx-2`}
                >
                  <MdRestore
                    fill={selectThemeClass("#000", "#fff")}
                    fontSize={35}
                  ></MdRestore>
                </button>
                <button
                  onClick={saveConnectionInfo}
                  className={`flex flex-row items-center justify-center bg-green-500 h-16 w-16 rounded-full p-2 mx-2`}
                  >
                    <MdSave
                      fill={selectThemeClass("#000", "#fff")}
                      fontSize={35}
                    ></MdSave>
                </button>
              </div>
            </div>
        </div>
            <div className="h-1/6 flex items-center justify-center align-center">
              <GoToButton
                fnGoTo={handleGoToSettings}
                goToSectionTitle="Configuracion"
                icon={<MdSettings
                  fill={selectThemeClass("#000", "#fff")}
                  size={30}
                ></MdSettings>}
                classnames="text-2xl ml-2 font-bold"
                classnamesContainer={`${selectThemeClass(
                  "bg-gray-100 text-black",
                  "bg-gray-900 text-white"
                )} px-5 flex-row`}
              ></GoToButton>
            </div>
          </div>
    </div>
  );
}

export default Home;
