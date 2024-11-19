<?php
include "database.php";

$dni = isset($_POST['int_dni_usuario']) ? $_POST['int_dni_usuario'] : '';
$nombre = isset($_POST['txt_nombre_usuario']) ? $_POST['txt_nombre_usuario'] : '';
$conexion = conexion();

/* ----------------------------------------------------------------------------------------------- */

registro($dni, $nombre, $conexion);