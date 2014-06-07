$(document).ready(function(e)
{

});

$("#on-btn-feedback").click(function(e)
{
	e.preventDefault();

	var email = $("#email").val();
	var message = $("#message").val();

	if (message.length == 0)
	{
		$.notify("Введите сообщение для того, чтобы оставить отклик.", {
			position: "bottom right"
		});
		$("#message").focus();
		return;
	}

	$("#on-btn-feedback").prop("disabled", true);

	$.post("api/send-feedback.php", 
	{
		email : email,
		message : message
	}, function(data)
	{
		if (data.success)
		{
			$.notify("Отклик отправлен.", {
				position: "bottom right",
				className : "success"
			});	
		}
		else
		{
			if (data.err_code == 1)
			{
				$.notify("Переданы не все параметры.", {
					position: "bottom right"
				});
			}
			else if (data.err_code == 2)
			{
				$.notify("Передан пустой параметр.", {
					position: "bottom right"
				});
			}
			else if (data.err_code == 3)
			{
				$.notify("Не удалось отправить сообщение.", {
					position: "bottom right"
				});
			}				
			else
			{
				$.notify("При отправке отклика произошла неизвестная ошибка.", {
					position: "bottom right"
				});
			}
		}
	}, "json").fail(function()
	{

	}).always(function()
	{
		$("#on-btn-feedback").prop("disabled", false);
	});
});