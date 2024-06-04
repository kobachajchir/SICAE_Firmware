export class WebSocketRepository {
    private websocket: WebSocket | null = null;
    private gateway: string;
    private onOpenCallback: ((event: Event) => void) | null = null;
    private onCloseCallback: ((event: CloseEvent) => void) | null = null;
    private onMessageCallback: ((event: MessageEvent) => void) | null = null;
  
    constructor(gateway: string) {
      this.gateway = gateway;
    }
  
    public getGateway() {
      return this.gateway;
    }

    public initWebSocket() {
      console.log('Trying to open a WebSocket connection...');
      this.websocket = new WebSocket(this.gateway);
      this.websocket.onopen = this.onOpen.bind(this);
      this.websocket.onclose = this.onClose.bind(this);
      this.websocket.onmessage = this.onMessage.bind(this);
    }
  
    public closeWebSocket() {
      if (this.websocket) {
        this.websocket.close();
      }
    }
  
    private onOpen(event: Event) {
      console.log('Connection opened');
      if (this.onOpenCallback) {
        this.onOpenCallback(event);
      }
    }
  
    private onClose(event: CloseEvent) {
      console.log('Connection closed');
      setTimeout(() => this.initWebSocket(), 2000);
      if (this.onCloseCallback) {
        this.onCloseCallback(event);
      }
    }
  
    private onMessage(event: MessageEvent) {
      if (this.onMessageCallback) {
        this.onMessageCallback(event);
      }
    }
  
    public setOnOpenCallback(callback: (event: Event) => void) {
      this.onOpenCallback = callback;
    }
  
    public setOnCloseCallback(callback: (event: CloseEvent) => void) {
      this.onCloseCallback = callback;
    }
  
    public setOnMessageCallback(callback: (event: MessageEvent) => void) {
      this.onMessageCallback = callback;
    }
  
    public sendMessage(message: string) {
      if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
        this.websocket.send(message);
      }
    }
  }
  