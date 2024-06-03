import React, { createContext, useContext, useState, useEffect } from "react";
import { fetchEnchufes, fetchSystemInfo } from "./../tools/api";
import { Enchufe, ApiResponse } from "../types/APITypes";

interface EspContextProps {
  systemInfo: ApiResponse["systemInfo"] | null;
  enchufesData: Enchufe[];
  refreshData: () => void;
}

const EspContext = createContext<EspContextProps>({
  systemInfo: null,
  enchufesData: [],
  refreshData: () => {},
});

export const EspProvider: React.FC<{ children: React.ReactNode }> = ({
  children,
}) => {
  const [systemInfo, setSystemInfo] = useState<
    ApiResponse["systemInfo"] | null
  >(null);
  const [enchufesData, setEnchufesData] = useState<Enchufe[]>([]);

  const refreshData = async () => {
    try {
      const enchufes = await fetchEnchufes();
      setEnchufesData(enchufes);
      const systemInfoResponse = await fetchSystemInfo();
      setSystemInfo(systemInfoResponse);
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  };

  useEffect(() => {
    refreshData();
  }, []);

  return (
    <EspContext.Provider value={{ systemInfo, enchufesData, refreshData }}>
      {children}
    </EspContext.Provider>
  );
};
