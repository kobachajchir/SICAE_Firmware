import React, {
  createContext,
  useContext,
  useState,
  useEffect,
  ReactNode,
} from "react";
import { espRepository } from "./../tools/espRepository";
import { User } from "../types/UserType";

// Tipo para el contexto de usuario
interface UserContextType {
  user: User | null;
  login: (username: string, password: string) => Promise<void>;
  logout: () => void;
  prefersDarkMode: boolean;
  setPrefersDarkMode: (value: boolean) => void;
}

// Crear el contexto
const UserContext = createContext<UserContextType | undefined>(undefined);

// Hook personalizado para usar el contexto
export const useUser = () => {
  const context = useContext(UserContext);
  if (!context) {
    throw new Error("useUser must be used within a UserProvider");
  }
  return context;
};

// Proveedor del contexto
export const UserProvider: React.FC<{ children: ReactNode }> = ({
  children,
}) => {
  const [user, setUser] = useState<User | null>(null);
  const [prefersDarkMode, setPrefersDarkMode] = useState<boolean>(false);

  useEffect(() => {
    // Aquí podrías cargar el usuario desde el almacenamiento local si es necesario
  }, []);

  const login = async (username: string, password: string) => {
    const user = await espRepository.login(username, password);
    if (user) {
      setUser(user);
      setPrefersDarkMode(user.prefersDarkMode);
    }
  };

  const logout = () => {
    setUser(null);
    setPrefersDarkMode(false);
    // Aquí podrías eliminar el usuario del almacenamiento local si es necesario
  };

  return (
    <UserContext.Provider
      value={{ user, login, logout, prefersDarkMode, setPrefersDarkMode }}
    >
      {children}
    </UserContext.Provider>
  );
};
