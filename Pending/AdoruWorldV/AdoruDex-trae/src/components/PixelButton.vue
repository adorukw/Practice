<script setup>
import { useRouter } from 'vue-router'
import { useCollectionStore } from '../stores/collection'

const store = useCollectionStore()
const router = useRouter()

defineProps({
  label: { type: String, default: '' },
  variant: { type: String, default: 'primary' },
  size: { type: String, default: 'md' },
  to: { type: String, default: '' },
})

const variantClasses = {
  primary: 'bg-pokemon-blue text-white hover:bg-pokemon-red',
  red: 'bg-pokemon-red text-white hover:bg-pokemon-yellow hover:text-pokemon-dark',
  yellow: 'bg-pokemon-yellow text-pokemon-dark hover:bg-pokemon-blue hover:text-white',
  green: 'bg-pokemon-green text-white hover:bg-pokemon-blue',
  outline: 'bg-pokemon-light text-pokemon-dark hover:bg-pokemon-blue hover:text-white',
}

const sizeClasses = {
  sm: 'px-3 py-1.5 text-[8px]',
  md: 'px-5 py-2.5 text-[10px]',
  lg: 'px-7 py-3 text-[12px]',
}

function handleClick(to) {
  if (to) {
    router.push(to)
  }
}
</script>

<template>
  <button
    :class="[
      'pixel-border font-pixel uppercase tracking-wider transition-all duration-200 active:translate-x-[2px] active:translate-y-[2px] active:shadow-none cursor-pointer',
      variantClasses[variant],
      sizeClasses[size],
    ]"
    @click="handleClick(to)"
  >
    <span class="flex items-center justify-center gap-2">
      <slot />
      {{ label }}
    </span>
  </button>
</template>
