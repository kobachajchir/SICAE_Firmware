import React, { ReactNode, createContext, useContext, useEffect, useRef } from 'react';
import { WebSocketRepository } from '../tools/websocketRepo';

interface WebSocketProviderProps {
    children: ReactNode;
}

const WebSocketContext = createContext<WebSocketRepository | null>(null);

export const WebSocketProvider: React.FC<WebSocketProviderProps> = ({ children }) => {
  const websocketRepoRef = useRef<WebSocketRepository | null>(null);

  if (!websocketRepoRef.current) {
    const gateway = `ws://${window.location.hostname}/ws`;
    websocketRepoRef.current = new WebSocketRepository(gateway);
  }

  useEffect(() => {
    const websocketRepo = websocketRepoRef.current;
    websocketRepo!.initWebSocket();

    return () => {
      websocketRepo!.closeWebSocket();
    };
  }, []);

  return (
    <WebSocketContext.Provider value={websocketRepoRef.current}>
      {children}
    </WebSocketContext.Provider>
  );
};

export const useWebSocket = () => {
  const context = useContext(WebSocketContext);
  if (!context) {
    throw new Error('useWebSocket must be used within a WebSocketProvider');
  }
  return context;
};
