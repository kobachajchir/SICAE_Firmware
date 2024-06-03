import { UserInfo, ConnectionInfo, SystemInfo } from "../types/APITypes";
import * as API from "./api";

export class ESPRepository {
  async login(data:UserInfo): Promise<UserInfo | null> {
    try {
      return await API.login(data);
    } catch (error) {
      console.error("Error during login:", error);
      return null;
    }
  }


}

export const espRepository = new ESPRepository();
