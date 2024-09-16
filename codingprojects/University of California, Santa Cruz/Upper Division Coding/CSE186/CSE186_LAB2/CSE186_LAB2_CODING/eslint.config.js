import js from "@eslint/js";
import globals from "globals";

export default [
    js.configs.recommended,
    {
        // rules: {
        //     "no-unused-vars": "warn",
        //     "no-undef": "warn",
        // },
        languageOptions: {
            ecmaVersion: 2024,
            sourceType: "module",
            globals: {
                ...globals.browser,
            }
        }
    }
];