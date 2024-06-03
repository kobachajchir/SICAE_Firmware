export interface SystemInfo {
  firmware: string;
  espTime: string;
  chipId: string;
}

export interface ConnectionInfo {
  wifiSsid: string;
  wifiPassword: string;
  apSsid: string;
  apPassword: string;
  apMode: boolean;
}

export interface UserInfo {
  userEmail: string;
  userPassword: string;
  prefersDarkMode: boolean;
}
