-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 11-10-2023 a las 07:48:50
-- Versión del servidor: 10.4.28-MariaDB
-- Versión de PHP: 8.0.28

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `id21247720_tacher_db`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `productos`
--

CREATE TABLE `productos` (
  `code_barras` bigint(20) NOT NULL,
  `Nombre` varchar(15) DEFAULT NULL,
  `valor_puntos` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `productos`
--

INSERT INTO `productos` (`code_barras`, `Nombre`, `valor_puntos`) VALUES
(1001, 'Producto 1', 34),
(1002, 'Producto 2', 22),
(1003, 'Producto 3', 123),
(1004, 'Producto 4', 54);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `puntos_usuarios`
--

CREATE TABLE `puntos_usuarios` (
  `id` bigint(20) NOT NULL,
  `dni_usuario` bigint(20) NOT NULL,
  `puntos` int(11) DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `puntos_usuarios`
--

INSERT INTO `puntos_usuarios` (`id`, `dni_usuario`, `puntos`) VALUES
(1, 12345678, 17),
(2, 12345677, 0),
(10, 11111113, 0);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `registro_deposito`
--

CREATE TABLE `registro_deposito` (
  `id` int(11) NOT NULL,
  `dni_usuario` bigint(20) NOT NULL,
  `code_producto` bigint(20) NOT NULL,
  `cantidad` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `usuarios`
--

CREATE TABLE `usuarios` (
  `dni` bigint(20) NOT NULL,
  `Nombre` varchar(15) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `usuarios`
--

INSERT INTO `usuarios` (`dni`, `Nombre`) VALUES
(11111113, 'prueba2'),
(12345677, 'tomas'),
(12345678, 'admin');

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `productos`
--
ALTER TABLE `productos`
  ADD PRIMARY KEY (`code_barras`);

--
-- Indices de la tabla `puntos_usuarios`
--
ALTER TABLE `puntos_usuarios`
  ADD PRIMARY KEY (`id`),
  ADD KEY `dni_usuario` (`dni_usuario`);

--
-- Indices de la tabla `registro_deposito`
--
ALTER TABLE `registro_deposito`
  ADD PRIMARY KEY (`id`),
  ADD KEY `dni_usuario` (`dni_usuario`),
  ADD KEY `code_producto` (`code_producto`);

--
-- Indices de la tabla `usuarios`
--
ALTER TABLE `usuarios`
  ADD PRIMARY KEY (`dni`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `puntos_usuarios`
--
ALTER TABLE `puntos_usuarios`
  MODIFY `id` bigint(20) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT de la tabla `registro_deposito`
--
ALTER TABLE `registro_deposito`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- Restricciones para tablas volcadas
--

--
-- Filtros para la tabla `puntos_usuarios`
--
ALTER TABLE `puntos_usuarios`
  ADD CONSTRAINT `puntos_usuarios_ibfk_1` FOREIGN KEY (`dni_usuario`) REFERENCES `usuarios` (`dni`);

--
-- Filtros para la tabla `registro_deposito`
--
ALTER TABLE `registro_deposito`
  ADD CONSTRAINT `registro_deposito_ibfk_1` FOREIGN KEY (`dni_usuario`) REFERENCES `usuarios` (`dni`),
  ADD CONSTRAINT `registro_deposito_ibfk_2` FOREIGN KEY (`code_producto`) REFERENCES `productos` (`code_barras`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
