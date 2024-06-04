const HtmlWebpackPlugin = require("html-webpack-plugin");
const TerserPlugin = require("terser-webpack-plugin");
const path = require("path");

module.exports = {
  entry: "./reactApp/index.tsx",
  output: {
    filename: "bundle.js",
    path: path.resolve(__dirname, "data"),
    publicPath: "/", // Importante para React Router
  },
  optimization: {
    minimize: true,
    minimizer: [new TerserPlugin()],
  },
  resolve: {
    extensions: [".tsx", ".ts", ".js", ".css"],
    alias: {
      "react-native$": "react-native-web",
    },
  },
  devServer: {
    static: {
      directory: path.resolve(__dirname, "data"),
    },
    port: 3000,
    open: true,
    hot: true,
    compress: true,
    historyApiFallback: true,
  },
  module: {
    rules: [
      {
        test: /\.css$/i,
        include: path.resolve(__dirname, "reactApp"),
        use: ["style-loader", "css-loader", "postcss-loader"],
      },
      {
        test: /\.(ts|tsx)$/,
        exclude: /node_modules/,
        use: "babel-loader",
      },
      {
        test: /\.ttf$/,
        loader: "url-loader",
        include: path.resolve(__dirname, "node_modules/react-web-vector-icons"),
      },
      {
        test: /\.(jpe?g|png|gif|svg)$/i,
        use: ["url-loader?limit=10000", "img-loader"],
      },
    ],
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: "./data/index.html",
    }),
  ],
};
