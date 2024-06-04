const HtmlWebpackPlugin = require("html-webpack-plugin");
const TerserPlugin = require("terser-webpack-plugin");
const BundleAnalyzerPlugin =
  require("webpack-bundle-analyzer").BundleAnalyzerPlugin
const path = require("path");

module.exports = {
  entry: "./index.tsx",
  output: {
    filename: "bundle.js",
    path: path.resolve(__dirname, "../data"),
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
      directory: path.resolve(__dirname, "../data"),
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
        include: path.resolve(__dirname, "./"),
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
        include: path.resolve(__dirname, "node_modules/@react-icons/all-files"),
        options: {
          limit: 10000,
          name: "fonts/[name].[ext]",
        },
      },
      {
        test: /\.(jpe?g|png|gif|svg)$/i,
        use: ["url-loader?limit=10000", "img-loader"],
      },
    ],
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: "./../data/index.html",
    }),
    //new BundleAnalyzerPlugin(),
  ],
};
