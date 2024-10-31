import { defineConfig } from 'vitepress'
import { components, navComponents } from './items'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "ElaWidgetTools Docs",
  description: "ElaWidgetTools 文档",
  lastUpdated: true,
  search: {
    provider: 'local',
  },
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'Home', link: '/' },
      { text: '上手指南', link: '/guide' },
      { text: '组件', items: navComponents },
      { text: '关于', link: '/about' },
    ],

    sidebar:{
      '/components/': [
        {
          text: '组件',
          items: components,
        },
      ],
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/vuejs/vitepress' }
    ],

    
  },
  markdown: {
    theme: {
      light: 'vitesse-light',
      dark: 'vitesse-dark',
    },
  },
})
