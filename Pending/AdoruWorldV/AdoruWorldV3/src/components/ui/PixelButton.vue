<script setup lang="ts">
defineProps<{
  variant?: 'primary' | 'secondary' | 'danger' | 'success'
  size?: 'sm' | 'md' | 'lg'
  disabled?: boolean
}>()

const variantClasses = {
  primary: 'bg-pokemon-red text-white hover:bg-pokemon-orange',
  secondary: 'bg-pokemon-blue text-white hover:bg-pokemon-purple',
  danger: 'bg-pokemon-black text-white hover:bg-pokemon-dark-gray',
  success: 'bg-pokemon-green text-pokemon-black hover:bg-pokemon-yellow'
}

const sizeClasses = {
  sm: 'text-sm px-3 py-2',
  md: 'text-base px-4 py-3',
  lg: 'text-lg px-6 py-4'
}
</script>

<template>
  <button
    class="pixel-btn"
    :class="[variantClasses[variant || 'primary'], sizeClasses[size || 'md']]"
    :disabled="disabled"
  >
    <slot />
  </button>
</template>
