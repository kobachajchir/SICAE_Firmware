// tailwind.config.js
module.exports = {
  purge: ["./reactApp/**/*.{js,jsx,ts,tsx}", "./data/index.html"],
  media: false, // or 'media' or 'class'
  theme: {
    extend: {},
  },
  variants: {
    extend: {},
  },
  plugins: [],
};
